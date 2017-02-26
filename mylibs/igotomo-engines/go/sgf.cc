#include "sgf.h"
#include "intern/go_libs.h"
#include "library/igx_string.h"
#include "library/igx_stdlib.h"
#include "io/igx_file.h"

namespace go { 

const stone_t kNullStone = { 0 };
const stone_t kStoneError = {static_cast<bit32_t>(0xFFFFFFFF)};

static const bit32_t kLengthStone (6); /* ;B[ab] + '\n' */
static const bit32_t kLengthLabel (8); /* LB[ab:A] */
static const bit32_t kLengthAddedStone  (6); /* AB[ab] */
static const bit32_t kLengthBranch (2); /* () */
static const bit32_t kLengthRemainTime (2); /* OT[3600000] */

/** sgf 出力に使う最大のバッファーサイズ **/
#define MAX_SGF_BUFFER_SIZE (4096)
#define LIMITED_SGF_FILE_SIZE (1024*1024*20)

/** 苦肉の策 **/
static const igx_mem_page_option_t string_page_option = {true, false, 1};
sgf::sgf()
: m_mem_string_page(&string_page_option),m_mem_stone_page(0), m_sgf(0), m_converted_string(0), m_expaned_allocate_memory(0), m_expected_length(0)
{
    memset(&m_header, 0, sizeof(m_header));
    memset(&m_top, 0, sizeof(m_top));
}
sgf::sgf(IgxMemPage* mem_page)
: m_mem_string_page(&string_page_option),m_mem_stone_page(0), m_sgf(0), m_converted_string(0), m_expaned_allocate_memory(0), m_expected_length(0)
{
    memset(&m_header, 0, sizeof(m_header));
    memset(&m_top, 0, sizeof(m_top));
    m_mem_stone_page=mem_page;
}

sgf::~sgf()
{
    Terminate();
}
bool_t sgf::Initialize(void* data, size_t size, void (*deallocator)(void*)/*=0*/)
{
    bool_t res=false;

    Terminate();
    res = Import((const char*)data, size, deallocator);

    if (deallocator!=0)
    {
        deallocator(data);
    }
    return res;
}
bool_t sgf::Initialize(const char* filename)
{
    Terminate();
    return Import(filename);
}
bool_t sgf::Initialize(const sgf_header_t& header)
{
    Terminate();
    m_header=header;
    return true;
}
void sgf::Terminate()
{
    memset(&m_header, 0, sizeof(m_header));
    memset(&m_top, 0, sizeof(m_top));

    if (m_expaned_allocate_memory)
    {
        igx_vfree(m_expaned_allocate_memory);
    }
    m_expaned_allocate_memory=0;
    m_expected_length=0;
    m_mem_string_page.Clear();
}
bool_t sgf::Import(const char* filename)
{
    /** 4k未満のファイルはスタックで読み込む*/
    char buffer[4096];
    char* p = buffer;
    const size_t size = igx_file_size(filename);

    if (size<sizeof(buffer))
    {
        if (igx_file_read(filename, buffer, 0, size)!=size)
        {
            IGX_ERROR("sgf::Import(%s) read failure", filename);
            return false;
        }
        buffer[size]=0;
    }
    else
    {
        size_t size;
        p = (char*)igx_file_load(filename, 0, &size, 0);
    }
    return Import(p, size, (size<sizeof(buffer))?0:&igx_file_unload);
}
bool_t sgf::Import(const char* data, size_t size, void (*deallocator)(void*)/*=0*/)
{
    char* p = (char*)parse_header(data, size);
    bool_t ret=true;

    if (p==0)
    {
        ret=false;
        goto PARSE_ERROR;
    }
    if (parse_body(p, &m_top, 0)==0)
    {
        ret=false;
        goto PARSE_ERROR;
    }
PARSE_ERROR:

    if (deallocator!=0)
    {
        (*deallocator)((void*)data);
    }

    return ret;
}
/**
* @brief TODO. メモリに直接書き込むのは効率が良いが、メモリのバッファーのサイズの取り方が難しい。
* \n内部ではストリームクラスを作成してそこに保持するのが現実的か。。。
*/
char* sgf::Export(char* buffer, size_t buffer_size, size_t* exported_size, const char* application_name, const char* version) const
{
    char* top_buffer=buffer;
    char* p = export_header(buffer, application_name, version);
    intptr_t header_size = (p-buffer);
    if (m_expected_length>(buffer_size-header_size))
    {
        /** メモリを仮想メモリにする **/
        IGX_INFO("expected size is %dbytes ... so, using virtual mmemory", m_expected_length);

        if (m_expaned_allocate_memory==0)
        {
            void** temp = (void**)&m_expaned_allocate_memory;
            *temp = igx_vmalloc(LIMITED_SGF_FILE_SIZE+1);
            IGX_ASSERT(m_expaned_allocate_memory);
        }
        top_buffer=p=(char*)m_expaned_allocate_memory;
        memcpy(p, buffer, header_size);
        p[header_size]=0;
        p+=header_size;
    }
    
    if ((p=export_body(p, &m_top))==0)
    {
        IGX_ERROR("sgf::Export failure!!\n");
        IGX_ASSERT(0);
        if (exported_size!=0)
        {
            *exported_size=0;
        }
        return 0;
    }
    if (exported_size!=0)
    {
        *exported_size=(size_t)(p-top_buffer);
    }

    return top_buffer;
}
bool_t sgf::ExportFile(const char* filename, const char* application_name, const char* version) const
{
    char buffer[MAX_SGF_BUFFER_SIZE+1];
    size_t size;
    char* p;
    if ((p=Export(buffer, sizeof(buffer), &size, application_name, version))!=0)
    {
        return (igx_file_write(filename, p, 0, size)==size);
    }

    return false;
}
ubit32_t sgf::AddStone(stone_t stone, float time/*=0.0*/)
{
    stone_node_t* next = m_curr->next;

    if (next==0)
    {
        /* 次のノードがなく、ブランチがある場合 */
        if (m_curr->branches!=0&& m_curr->branches->Count()!=0)
        {
            /** この場合はブランチあり **/
            for (ItxNode<stone_node_t*>* node=m_curr->branches->GetTopFrom(); node; node=node->next)
            {
                /* 同じ所に石があれば、MoveStone扱い */
                if (stone==(**node)->data.stone)
                {
                    return MoveNext(stone)->data.stone.order;
                }
            }

        }
    }
    else
    {
        /** ブランチ無し**/
        if (m_curr->next->data.stone==stone)
        {
            /** この場合は再生扱い。**/
            return MoveNext(stone)->data.stone.order;
        }
        else
        {
            /** 次のブランチはあるが、別の所に入力する場合なのでブランチ扱い **/
            if (m_curr->branches!=0)
            {
                /** あり得ないケース*/
                IGX_ASSERT(m_curr->branches->Count()==0);
            }
            else
            {
                m_curr->branches = new(m_mem_stone_page) ItxLinkedList<stone_node_t*>(8);
                /** この場合は次のノードをこちらに移動させる必要がある。**/
                m_curr->branches->Add(next);
                next->prev=0;
                next->parent=m_curr;
                m_curr->next=0;
                m_expected_length+=kLengthBranch;
            }
        }
   }

    next=(stone_node_t*)pmalloc(m_mem_stone_page, sizeof(stone_node_t));
    memset(next, 0, sizeof(stone_node_t));

    next->data.stone=stone;
    next->data.stone.order = m_curr->data.stone.order+1;

    m_expected_length+=kLengthStone;
    m_expected_length+=((time!=0)?kLengthRemainTime:0);

    if (m_curr->branches!=0)
    {
        next->next=0;
        next->prev=0;
        next->parent=m_curr;
        m_curr->next=0;
        m_curr->branches->Add(next);
    }
    else
    {
        m_curr->next=next;
        next->prev=m_curr;
    }
    next->data.stone=stone;
    next->data.remain_time=time;
    next->data.stone.order=m_curr->data.stone.order+1;
    m_curr = next;
    return m_curr->data.stone.order;
}
void sgf::RemoveNode()
{
    stone_node_t* node = m_curr;

    if (node==0)
    {
        return ;
    }
    m_curr->next=0;
    /** メモリプールを使っているので、実際には削除しなくてよい 
        これで良いの？
        改良版は今後の課題にして、とりあえずバージョン v1.0.0ではこれで行く。
    **/
}
ubit32_t sgf::AddLabel(label_t label)
{
    if (m_curr->data.label==0)
    {
        m_curr->data.label = new(m_mem_stone_page) ItxLinkedList<label_t>(4);
    }
    for (ItxNode<label_t>* node=m_curr->data.label->GetTopFrom(); node; node=node->next)
    {
        label_t at = **node;
        if (at.x==label.x&&at.y==label.y)
        {
            /** 削除 */

            m_expected_length-=kLengthLabel;
            m_curr->data.label->RemoveNode(node);
            return m_curr->data.label->Count();
        }
    }
    m_expected_length+=kLengthLabel;
    return m_curr->data.label->Add(label);
}
ubit32_t sgf::AddInitStone(stone_t stone)
{
    if (m_curr->data.initial_stone_list==0)
    {
        m_curr->data.initial_stone_list = new(m_mem_stone_page) ItxLinkedList<stone_t>(32);
    }
    for (ItxNode<stone_t>* node=m_curr->data.initial_stone_list->GetTopFrom(); node; node=node->next)
    {
        stone_t at = **node;
        if (at.x==stone.x&&at.y==stone.y)
        {
            /** 削除 */
            m_expected_length-=kLengthAddedStone;
            m_curr->data.initial_stone_list->RemoveNode(node);
            return m_curr->data.initial_stone_list->Count();
        }
    }
    m_expected_length+=kLengthAddedStone;
    return m_curr->data.initial_stone_list->Add(stone);
}
void sgf::AddComment(const char* comment, size_t len/*=0*/)
{
    int skip_char_count = libs::get_skip_char_count(comment);
    int dest_count=0;
    len=(len==0)?strlen(comment):len;
    m_curr->data.comment = (char*)pmalloc(&m_mem_string_page, len+1+skip_char_count);
    m_expected_length+=(len+1+skip_char_count);

    for (int i=0; i<len; ++i)
    {
        if (libs::is_skip_char(comment[i])==true)
        {
            m_curr->data.comment[dest_count++]='\\';
        }
        m_curr->data.comment[dest_count++]=comment[i];
    }
    m_curr->data.comment[dest_count]=0;
}
stone_node_t* sgf::MoveTop()
{
    m_curr=&m_top;
    return m_curr;
}
stone_node_t* sgf::MoveNext(stone_t next_stone)
{
    if (m_curr->next==0)
    {
        if (m_curr->branches!=0&&m_curr->branches->Count()!=0)
        {
            ItxNode<stone_node_t*>* node = m_curr->branches->GetTopFrom();
            if (IS_NULL_STONE(next_stone)==true)
            {
                m_curr=**node;
                return **node;
            }
            for (;node;node=node->next)
            {
                stone_node_t* stone = **node;
                if (stone->data.stone==next_stone)
                {
                    m_curr=stone;
                    return stone;
                }
            }
            /** どうする **/
            IGX_ASSERT(false);
        }
        return m_curr;
    }
    m_curr=(m_curr->next!=0)?m_curr->next:m_curr;

    return m_curr;
}
stone_node_t* sgf::MovePrev()
{
    if (m_curr->prev==0&&m_curr->parent==0)
    {
        /** top **/
        return m_curr;
    }
    else if (m_curr->prev!=0)
    {
        m_curr=m_curr->prev;
        return m_curr;
    }

    IGX_ASSERT(m_curr->parent);
    m_curr=m_curr->parent;
    return m_curr;
}
bool_t sgf::RemoveInitStone(stone_t stone)
{
    if (m_curr->data.initial_stone_list!=0)
    {
        ItxNode<stone_t>* node = m_curr->data.initial_stone_list->GetTopFrom();
        for (;node;node=node->next)
        {
            if (stone==**node)
            {
                m_curr->data.initial_stone_list->RemoveNode(node);
                m_expected_length-=kLengthAddedStone;
                return true;
            }
        }
    }
    return false;
}

} /** namespace go **/
