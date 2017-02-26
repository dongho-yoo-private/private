#include "igx_types.h"
#include "go/go_types.h"
#include "go/engine.h"
#include "stdio.h"

go::engine* engine_addr;

static void show_help()
{
    printf("command list ----------------\n");
    printf("A: AddStone\n");
    printf("E: Edit header\n");
    printf("S: Save\n");
    printf("LD: Load\n");
    printf("MT: MoveTop\n");
    printf("ME: MoveEnd\n");
    printf("MN: MoveNext\n");
    printf("MP: MovePrev\n");
    printf("P: Play\n");
    printf("B: Back\n");
    printf("L: Label\n");
    printf("I: No order stone\n");
    printf("SB: Show block\n");
    printf("exit: quit\n");
    printf("help: help\n");
}

static bool_t add_stone()
{
    int  x, y;
    
    printf("Enter x y: ");
    scanf("%d %d", &x, &y);

    if (engine_addr->AddStone(x-1, y-1)==kSTONE_ERROR)
    {
        return false;
    }

    engine_addr->__print_text_goban__();

    return true;
}
static bool_t move_next()
{
    if (engine_addr->MoveFromCurrent(1)!=0)
    {
        engine_addr->__print_text_goban__();
        const char* comment = engine_addr->GetComment();
        if (comment)
        {
            printf("comment:%s\n", comment);
        }
        return true;
    }
    go::stone_t list[32];
    bit32_t n = engine_addr->GetBranch(list, 32);

    if (n==0)
    {
        return false;
    }

    for (int i=0; i<n; ++i)
    {
        printf("%c: %d,%d\n", 'A'+i, list[i].x, list[i].y);
    }
    printf("select branch: ");
    char ch;
    scanf("%c", &ch);
    if (engine_addr->AddStone(list[ch-'A'].x, list[ch-'A'].y)!=kSTONE_ERROR)
    {
        engine_addr->__print_text_goban__();
        return true;
    }
    return false;
}
static bool_t move_prev()
{
    if (engine_addr->MoveFromCurrent(-1)!=0)
    {
        engine_addr->__print_text_goban__();
        return true;
    }
    return false;
}

static bool_t show_block()
{
    engine_addr->__show_block__();
    return true;
}
static bool_t save()
{
    char filename[256];
    go::sgf& _sgf = engine_addr->GetSgf();
    printf("filename: ");
    scanf("%s", filename);
    remove(filename);
    if (_sgf.ExportFile(filename, "go_engine_sample", "v0.1.0")==true)
    {
        IGX_INFO("%s created!!", filename);
        return true;
    }
    IGX_ERROR("export failure!!");
    return false;
}
static bool_t load()
{
    char filename[256];
    printf("filename: ");
    scanf("%s", filename);
    go::engine* engine = new ("go_engine_sample") go::engine();
    engine_addr=engine;
    bool_t is_success = engine->Initialize(filename);

    if (is_success==false)
    {
        IGX_ERROR("load failure!\n");
        return false;
    }
    engine_addr->__print_text_goban__();
    return true;
}
static bool_t dispatch_command(const char* command)
{
#define COMMAND_CALL(cmd, func) \
if (strcmp(command, cmd)==0)\
{\
    return func();\
}
    COMMAND_CALL("A", add_stone);
    COMMAND_CALL("SB", show_block);
    COMMAND_CALL("S", save);
    COMMAND_CALL("LD", load);
    COMMAND_CALL("MN", move_next);
    COMMAND_CALL("MP", move_prev);

    return false;
}
static void make_default_header(go::sgf_header_t& header, const char* black_name, const char* white_name)
{
    header.player[0].name=(char*)black_name;
    header.player[1].name=(char*)white_name;
}

int main(int argc, char** argv)
{
    igx_log_init(kLogOutputConsole,kLogLevelAll, 0);

    go::engine _engine;
    go::sgf_header_t header = {0, };
    engine_addr = &_engine;

    make_default_header(header, "black", "white");
    _engine.Initialize(header);

    show_help();
    char command[16];
    do
    {
        printf("InputCommand: ");
        scanf("%s", command);

        if (dispatch_command(command)==false)
        {
            show_help();
        }

    } while(strcmp(command, "exit")!=0);
    return 0;
}
