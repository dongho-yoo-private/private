
#ifdef _GOYOTSUYA

// --------------------------------------
Bool CGoEditorApp::OnCommand(int nSystemId, cGlassInterface* from, IntPtr param1, LongPtr param2)
{
	switch(nSystemId)
	{
		//eGCID_PP,
		//eGCID_PREV,
		//eGCID_NEXT,
		//eGCID_NN,
		//eGCID_END,
		//eGCID_SHOW_NO,
		//eGCID_NEXT_BRANCH,
		//eGCID_PREV_BRANCH,
		//eGCID_DELETE_NODE,
		//eGCID_PASS,
		//eGCID_WAIT,
		//eGCID_HUU,
		case eGCID_MSG_MAIN_UDP:
		{
			xOnMainUdpPacket(param1, (void*)param2);
			break;
		}
		case eGCID_ROUTER_FOUND:
		{
			//::yiTraceM2("c:\\yotsuya\\upnp.log", "OnCommand(eGCID_ROUTER_FOUND)! %x", m_hUpnp);
			if (m_hUpnp!=0)
			{
				int size;
				const void* p = ::yiupnpExport(m_hUpnp, &size);
				char szUpnpPath[256];
				wsprintf(szUpnpPath, "%s\\%s", m_szInstDir, "upnp");

				yiFileWrite(szUpnpPath, (void*)p, 0, size);

				if (m_hUpnp)
				{
					//::yiTraceM2("c:\\yotsuya\\upnp.log", "OnCommand(eGCID_ROUTER_FOUND)! xRegisterUpnpPort before");
					xRegisterUpnpPort();
					//::yiTraceM2("c:\\yotsuya\\upnp.log", "OnCommand(eGCID_ROUTER_FOUND)! xRegisterUpnpPort after");
				}
			}

			ConnectMyFriends();


			m_bIsFindRouterDone=True;

			break;
		}
		case eGCID_STONE_MESSAGE:
		{
			wchar_t stone_comment[256];
			stone_comment[0]=0;
			int goid=0;

			if (from==m_goban)
			{
				goid=param1&0xFF;
				wchar_t* msg=(wchar_t*)param2;
				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(goid);

				int i=(param1>>8)&0xFF;
				int j=(param1>>16)&0xFF;

				if (info && info->hUser)
				{
					int len = (wcslen(msg)+1)<<1;
					sGeneralParam* p = (sGeneralParam*)yialloc(len+sizeof(int)+sizeof(int));
					p->param[0]=i;
					p->param[1]=j;
					wcscpy((wchar_t*)&p->param[2], msg);
					m_network->SendToUserEx(info->hUser, eGNPMSG_STONE_MESSAGE, p, len+(sizeof(int)<<1));
				}

				wchar_t szMyName[16];
				yiToUnicode(szMyName, CUserManager::GetInstance()->GetMyName(), 0);
				wsprintfW(stone_comment, L"%d:%s[%c,%c]> %s\n", m_goban->GetCurrentOrder(goid), szMyName, (char)(i+'a'), (char)(j+'A'), msg);
			}
			else
			{
				unsigned int* param = (unsigned int*)param1;
				sGoUserInfo* info = (sGoUserInfo*)param[0];
				sGeneralParam* p = (sGeneralParam*)&param[2];
				wchar_t* msg = (wchar_t*)&p->param[2];
				int i=p->param[0];
				int j=p->param[1];

				i=19-i-1;
				j=19-j-1;
				
				goid=info->goid;
				m_goban->ShowBaloon(info->goid, msg, i, j);

				wchar_t szUserName[16];
				yiToUnicode(szUserName, info->szName, 0);
				wsprintfW(stone_comment, L"%d:%s[%c:%c]> %s\n", m_goban->GetCurrentOrder(info->goid), szUserName, (char)(i+'a'), (char)(j+'A'), msg);

				yifree(param);


			}

			OnCommand(eGCID_TEXT_MESSAGE, 0, goid, (LongPtr)stone_comment);

			break;
		}
		case eGCID_LABEL:
		{
			if (param1!=CBN_SELCHANGE)
			{
				break;
			}
			cglsSplitButton* btn = (cglsSplitButton*)from;

			int id = btn->GetSelectedId();
			m_goban->SetLabelMode((eLabelType)id);
			break;
		}
		case eGCID_AUTOPLAY:
		{
			break;
		}
		case eGCID_SHOW_NO:
		{
			if (param1!=CBN_SELCHANGE)
			{
				break;
			}
			cglsSplitButton* btn = (cglsSplitButton*)from;

			int index = btn->GetSelectIndex();			

			if (index==0)
			{
				m_goban->ShowNo(False);
			}
			else
			{
				m_goban->ShowNo(True);
			}
			break;
		}
		case eGCID_DELETE_NODE:
		{
			m_goban->DeleteNode(0);
			break;
		}
		case eGCID_TOP:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(from==0?param2:0, MOVE_TOP, list, (from==0));
			if (stone==(stone_t** )0xFFFFFFFF)
			{
				OnCommand(eGCID_SEND_CMD, 0, nSystemId, m_goban->GetCurrentGoId());
			}
			//m_goban->NotifyNodeChanged();
			break;
		}
		case eGCID_NEXT:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, 1, list, (from==0));
			if (stone==(stone_t** )0xFFFFFFFF)
			{
				OnCommand(eGCID_SEND_CMD, 0, nSystemId, m_goban->GetCurrentGoId());
			}
			//m_goban->NotifyNodeChanged();
			break;
		}
		case eGCID_PP:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, -10, list, (from==0));
			if (stone==(stone_t** )0xFFFFFFFF)
			{
				OnCommand(eGCID_SEND_CMD, 0, nSystemId, m_goban->GetCurrentGoId());
			}
			//m_goban->NotifyNodeChanged();
			break;
		}
		case eGCID_PREV:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, -1, list, (from==0));
			if (stone==(stone_t** )0xFFFFFFFF)
			{
				OnCommand(eGCID_SEND_CMD, 0, nSystemId, m_goban->GetCurrentGoId());
			}
			//m_goban->NotifyNodeChanged();
			break;
		}
		case eGCID_NN:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, 10, list, (from==0));
			if (stone==(stone_t** )0xFFFFFFFF)
			{
				OnCommand(eGCID_SEND_CMD, 0, nSystemId, m_goban->GetCurrentGoId());
			}
			//m_goban->NotifyNodeChanged();
			break;
		}
		case eGCID_END:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, MOVE_END, list, (from==0));

			if (stone==(stone_t** )0xFFFFFFFF)
			{
				OnCommand(eGCID_SEND_CMD, 0, nSystemId, m_goban->GetCurrentGoId());
			}
			//m_goban->NotifyNodeChanged();
			break;
		}
		case eGCID_NEXT_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoNextBranch(m_goban->GetCurrentGoId(), branches);
			break;
		}
		case eGCID_PREV_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoPrevBranch(m_goban->GetCurrentGoId(), branches);
			break;
		}
		case eGCID_ADD_NEW:
		{
			unsigned int flag=0;
			unsigned int condition=cyiGoEngine::DefaultCondition(flag);
			m_goban->AddForEdit(0, 0, 0, condition, flag);
			break;
		}
		case eGCID_PASS:
		{
			m_goban->PutStone(0, eST_PASS, 0);
			break;
		}
		case eGCID_CURRENT_SAVE:
		{
			if (m_goban->Save(0, eGFT_SGF, param1==0?-1:param1)==False)
			{
				if (m_myId!=0)
				{
					char szFileName[256];
					char path[256];
					unsigned int day  = m_goban->GetUniversalGoid(0)->day;
					unsigned int time = m_goban->GetUniversalGoid(0)->time;

					wsprintf(szFileName, "%d%02d%02d%02d%02d%02d.sgf", DAY_YEAR(day), DAY_MON(day), DAY_DAY(day),
																	xHOUR(time), xMINUTE(time), xSECOND(time));
					wsprintf(path, "%s\\data\\other\\%s", m_szUserDir, szFileName);

					Bool res = m_goban->Save(path);

					if (res==False)
					{
						break;
					}

					m_goban->SetChanged(param1, False);//	m_tab->SetItemChanged(eng->m_id, eng->IsChanged());

					sGoHeaderEx hd;
					if (m_goban->ReadHeader(path, &hd)==False)
					{
						// 何か問題？
						return False;
					}
					SaveGoData(1, &hd.header, szFileName, hd.map, hd.n);
					break;
				}
				// ログイン状態じゃない場合。
				else
				{
					char szFileName[256];
					char path[256];
					unsigned int day  = m_goban->GetUniversalGoid(0)->day;
					unsigned int time = m_goban->GetUniversalGoid(0)->time;

					wsprintf(szFileName, "%d%02d%02d%02d%02d%02d.sgf", DAY_YEAR(day), DAY_MON(day), DAY_DAY(day),
																	xHOUR(time), xMINUTE(time), xSECOND(time));
					wsprintf(path, "%s\\data\\other\\%s", m_szUserDir, szFileName);

					Bool res = m_goban->Save(path);

					if (res==False)
					{
						break;
					}

					m_goban->SetChanged(param1, False);//	m_tab->SetItemChanged(eng->m_id, eng->IsChanged());

					sGoHeaderEx hd;
					if (m_goban->ReadHeader(path, &hd)==False)
					{
						// 何か問題？
						return False;
					}
					SaveGoData(1, &hd.header, szFileName, hd.map, hd.n);
					break;
				}
			}
			else
			{
				sGoHeaderEx hd;
				if (m_goban->ReadHeader(0, &hd)==False)
				{
					// 何か問題？
					return False;
				}
				char* pszFileName = (char*)m_goban->GetFileName(0);

				SaveGoData(1, &hd.header, (char*)yiPathGetFileName(pszFileName), hd.map, hd.n);
				m_goban->SetChanged(param1, False);
			}

			break;
		}
		case eGCID_CURRENT_SAVE_AS:
		{
			//cglsMessageBox box;
			//box.Show(m_main, L"保存しますか？", L"", MB_OKCANCEL);
			//m_goban->ShowMessage(
			//if (m_goban->Save(0)==0)
			//{

			char szFileName[256];
			if (yiFileSaveDialog(*m_main, "保存", "", "*.sgf", "sgf(*.sgf)\0",  szFileName)==True)
			{
				m_goban->Save(szFileName);
				m_goban->SetChanged(param1, False);
			}
			//}
			break;
		}
		case eGCID_CLOSE:
		{
			m_goban->DeleteGoban(0, True);
			break;
		}
		case eGCID_COPY:
		{
			m_goban->CopyAndAdd();
			break;
		}
		case eGCID_CALCULATE_START:
		{
			m_goban->StartCalculateMode(0);
			break;
		}
		case eGCID_CALCULATE_CONFIRM:
		{
			if (from==m_main)
			{
				sGoUserInfo* info = (sGoUserInfo*)m_network->IndexToUser(param1);
				m_goban->ShowMessage(info->goid, L"相手から地合い計算終了が申し込まれました。\r\nよろしいでしょうか？\r\n", L"確認", MB_OKCANCEL, eGCID_CALCULATE_CONFIRM);
			}
			else if (from==m_goban)
			{
				int res = param1>>16;
				int id = param1&0xFFFF;

				int goid = m_goban->MessageIdToGoId(id);
				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(goid);

				if (res==IDOK)
				{
					OnCommand(eGCID_CALCULATE_END, 0, 0, 0);
					m_network->SendToUserEx(info->hUser, eGNPMSG_CALCULATE_END, 0, 0);
				}
				else // キャンセル
				{
					m_network->SendToUserEx(info->hUser, eGNPMSG_CALCULATE_NO_CONFIRM, 0, 0);
				}
			}
			else
			{
				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);
				
				if (info)
				{
					if (info->hUser)
					{
						m_network->SendToUserEx(info->hUser, eGNPMSG_CALCULATE_CONFIRM, 0, 0);
						m_goban->Lock(0);
					}
					else
					{
						// この場合は計算終了へ
						OnCommand(eGCID_CALCULATE_END, 0);
					}
				}
				else
				{
					OnCommand(eGCID_CALCULATE_END, 0);
				}
			}
			break;
		}
		case eGCID_CALCULATE_NO_CONFIRM:
		{
			sGoUserInfo* user = (sGoUserInfo*)m_network->IndexToUser(param1);

			if (user)
			{
				m_goban->Unlock(user->goid);
				m_goban->ShowMessage(user->goid, L"計算終了が断られました。", L"お知らせ", MB_OK);
			}
			break;
		}
		case eGCID_CALCULATE_END:
		{
			goid_t goid=0;

			if (from==m_main)
			{
				sGoUserInfo* user = (sGoUserInfo*)m_network->IndexToUser(param1);
				goid=user->goid;
				m_goban->Unlock(goid);
			}

			int winer;
			int score;
			m_goban->CalculateEnd(goid, 0, 0, winer, score, False);

			if (m_network==0)
			{
				m_goban->ChangeInputMode(eIM_EDIT);
				break;
			}

			unsigned int day=0;
			unsigned int time = cyiTime::GetLocalTime(day);
			char szNewFileName[256];
			wsprintf(szNewFileName, "%d%02d%02d%02d%02d%02d.sgf", DAY_YEAR(day), DAY_MON(day), DAY_DAY(day),
														xHOUR(time), xMINUTE(time), xSECOND(time));

			
			sGoPlayInfo* pInfo = (sGoPlayInfo* )m_goban->GetExtraData(goid);

			if (pInfo==0)
				break;

			sGoUserInfo* info = (sGoUserInfo*)pInfo->hUser;//(sGoUserInfo*)m_network->FindUser(0, 0, m_goban->GetCurrentGoId());

			if (info==0)
				break;

			int result=((winer<<29)|score);

			xRemovePlayingGo(info->id, *m_goban->GetUniversalGoid(info->goid), m_goban->ImBlack(info->goid));
			SendBroadCast(*m_goban->GetUniversalGoid(info->goid), eGNPMSG_GO_RESULT, &result, sizeof(result));

			if (info)
			{
				if (CUserManager::GetInstance()->IsMyFriend(info->id)==True ||
					CUserManager::GetInstance()->IsGroupUser(info->id)==True )
				{
					m_goban->ChangeInputMode(eIM_STUDY);
				}
				else
				{
					m_goban->ChangeInputMode(eIM_ENJOY);
					sGoPlayInfo* pInfo = (sGoPlayInfo*)m_goban->GetExtraData(info->goid);
					pInfo->hUser=0;
					info->goid=0;
				}
				SaveGoDataEx(m_goban->GetCurrentGoId());
			}
			/*else
			{
				m_goban->ChangeInputMode(eIM_ENJOY);
			}*/



			break;
		}
		case eGCID_CHATT:
		{
			const wchar_t* pszMessage = (const wchar_t*)param1;
			sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);//GetCurrentGoId();
			char szMessage[256];
			const char* pszName = m_goban->ImBlack()?m_goban->GetBlackName():m_goban->GetWhiteName();
			wsprintf(szMessage, "%d:%s> %S\n", m_goban->GetCurrentOrder(0), pszName, pszMessage);
			m_network->SendTextMessage((HandleUser)info->hUser, szMessage);
			m_goban->AppendComment(0, szMessage);
			return (Bool)::yiToUnicode(0, szMessage, 0);
		}
		case eGCID_TEXT_MESSAGE:
		{
			if (from==m_main)
			{
				unsigned int* p = (unsigned int*)param1;
				sGoUserInfo* info = (sGoUserInfo*)p[0];
				const char*str = strstr((const char*)&p[2], ">");
				wchar_t* pw = ::yiToUnicode(0, (const char*)&p[2], 0);
				m_goban->AppendMessage(info->goid, pw, p[1]);
				m_goban->AppendComment(info->goid, (const char*)&p[2]);

				if (str!=0)
				{
					yifree(pw);
					pw = ::yiToUnicode(0, str+1, 0);
				}
				m_goban->ShowBaloon(info->goid, pw, 9, 0);
				yifree(pw);
				yifree(p);

			}
			else
			{
				m_goban->AppendMessage(param1, (wchar_t*)param2, 0);

				char buffer[256];
				::yiToMultiByteCode(buffer, (wchar_t*)param2, 0);
				m_goban->AppendComment(param1, (const char*)buffer);
			}
			break;
		}
		case eGCID_REGISTER_USER:
		{
			if (m_server_addr==0)
			{
				if ((m_server_addr=xGetServerAddress(&m_server_port))==0)
				{
					cglsMessageBox msg;
					msg.Show(m_main, L"サーバへ接続失敗。", L"", MB_OK, &Font(L"Meiryo", 14.0));
					break;
				}
			}

			char szUserName[16];
			char szPassword[8];
			unsigned int id=xRegisterUser(szUserName, szPassword);

			if (id!=0)
			{
				SetProperty(0, "LastLoginUser", szUserName);
				//CUserManager::GetInstance()->AddDictionary(id, szUserName, 0);
			}

			PostCommand(eGCID_LOGIN, m_main, 0, 0);
			break;
		}
		case eGCID_LOGIN:
		{
			wchar_t wszId[16]={0,};
			wchar_t wszPass[8]={0,};
			char szUserName[16]={0,};
			char szPassword[8];
			Bool bIsNoUser=False;

			if (m_server_addr==0)
			{
				if ((m_server_addr=xGetServerAddress(&m_server_port))==0)
				{
					cglsMessageBox msg;
					msg.Show(m_main, L"サーバへ接続失敗。", L"", MB_OK, &Font(L"Meiryo", 14.0));
					break;
				}
			}

			cyigLoginDialog login;
			float fontSize=20.0*m_goban->GetScale();
			fontSize=fontSize<12.0?12.0:fontSize;

			if (GetProperty(0, "LastLoginUser", szUserName)==False)
			{
				if (from==0)
				{
					PostCommand(eGCID_REGISTER_USER, 0, 0, 0);
					break;
				}
			}

			yiToUnicode(wszId, szUserName, 0);

			if (login.Login(m_main, wszId, wszPass, 0, &Font(L"Meiryo", fontSize))==False)
			{
				return False;
			}

			yiToMultiByteCode(m_szMyId, wszId, 0);
			yiToMultiByteCode(szPassword, wszPass, 0);
			
			if (xLogin(m_szMyId, szPassword, login.IsNormalLogin())==False)
			{
				cglsMessageBox msg;
				msg.Show(m_main, L"ログインに失敗しました。", L"", MB_OK, &Font(L"Meiryo", 14.0));

				m_myId=0;
				m_szMyId[0]=0;
				break;
			}

			m_bIsLoginSuccess=True;
			m_system->EnableCommand(eGCID_LOGIN, False);

			CUserManager::GetInstance()->AddDictionary(m_myId, m_szMyId, 0);

			if (login.IsNormalLogin()==False)
			{
				char szGoFilePath[128];
				const sypnUserInfo* info = m_network->GetMyInfo();
				
				wsprintf(szGoFilePath, "%s\\data\\other\\.list", m_szUserDir);
				CUserManager::GetInstance()->SetGoFilePath(1, szGoFilePath);
				m_goban->UpdateMyView(0, 0, 0, eMVS_NOID);
				cglsToolContainer* con = m_goban->GetContainer();
				con->SetPannelStyle(eGTCS_AFTER_LOGIN);

				break;
			}
			// 通常ログイン
			else
			{
				char buffer[128];
				SetProperty(0, "LastLoginUser", m_szMyId);
				wsprintf(buffer, "%sさん Go!Yotsuyaへようこそ！", m_szMyId);
				SetWindowText(*m_main, buffer);

				// 登録時でないパソコンの場合ディレクトリがないかも知れないので
				// ディレクトリを作成しておく。

				// ユーザディレクトリ
				{
					char path[256];
					
					wsprintf(path, "%s\\data", m_szMyDir); 
					yiMakeDir(path);
					wsprintf(path, "%s\\data\\played", m_szMyDir); 
					yiMakeDir(path);
				}
				
				sBaseUserInfo* base = CUserManager::GetInstance()->GetMyBaseInfo();

				char szGoFilePath[256];
				wsprintf(szGoFilePath, "%s\\data\\other\\.list", m_szUserDir);
				CUserManager::GetInstance()->SetGoFilePath(1, szGoFilePath);

				m_goban->UpdateMyView(0, 0, 0, eMVS_NORMAL);
				//m_goban->SetMyView(&info, eMVS_NORMAL, CUserManager::GetInstance());
				cglsToolContainer* con = m_goban->GetContainer();
				con->SetPannelStyle(eGTCS_AFTER_LOGIN);

				int n;
				sMsg32* msg = CUserManager::GetInstance()->GetMyMsg32(&n);

				for (int i=0; i<n; i++)
				{
					if (msg[i].type!=0)
					{
						OnCommand(eGCID_NOTIFY_MSG32, 0, (unsigned int)&msg[i]);
					}
				}

			}
			break;
		}
		// 再接続時に黒の方でパケットを投げる。
		// 白の方にデータがなければ、データの取得リクエストを転送。
		case eGCID_REQ_REPLAY:
		{
			if (from==0)
			{
				sSimpleGoData* data = (sSimpleGoData*)param1;
				ubit32_t id = data->hd.remoteId;
				ubit32_t myId = CUserManager::GetInstance()->GetMyId();
				HandleUser hUser = (HandleUser)CUserManager::GetInstance()->GetUserHandle(id);

				if (hUser==0)
					return 0;
				int n = (data->hd.result>>6)&0x1FF;

				Bool bIAmBlack = (data->hd.result&0x80000000)?True:False;
				sGeneralParam s = {bIAmBlack?myId:id, bIAmBlack?id:myId, data->hd.day, data->hd.time, n, 0};
				m_network->SendToUserEx(hUser, eGNPMSG_REQ_REPLAY, &s, GP_SIZE(5));//data, (sizeof(sSimpleGoData)+(n<<1)));

				//yifree((void*)param1);
			}
			else if (from==m_main)
			{
				unsigned int* param = (unsigned int*)param1;
				sGoUserInfo* info = (sGoUserInfo*)param[0];
				ugoid_t* uid = (ugoid_t*)&param[2];

				ubit32_t userId = info->id;

				sSimpleGoHeader* data = CUserManager::GetInstance()->GetWaitGoHeader(userId, *uid);

				if (data==0)
				{
					bit32_t res=0xFFFFFFFF;
					m_network->SendToUser(info->index, eGNPMSG_REQ_REPLAY_OK, &res, sizeof(res));
					break;
				}

				cglsMessageBox msg;
				wchar_t wszMsg[128];

				// この場合は打ち掛け碁
				if (((data->result>>6)&0x1FF)!=0)
				{
					wchar_t wszUserName[16];
					wsprintfW(wszMsg, L"%sさんと打ち掛け碁があります。\r\n対局しますか？", yiToUnicode(wszUserName, info->szName, 0));
				}
				// 残りは予定碁
				else
				{
				}

				int res = msg.Show(m_main, wszMsg, L"お知らせ", MB_OKCANCEL, __FONT_MESSAGEBOX__);

				if (res!=IDOK)
				{
					bit32_t x=1;
					m_network->SendToUser(info->index, eGNPMSG_REQ_REPLAY_OK, &x, sizeof(x));
					break;
				}
				
				int size;
				sSimpleGoData* godata = CUserManager::GetInstance()->GetGoData(*uid, size, True);

				// ヘッダーはあるのに棋譜がない。
				if (godata==0)
				{
					DebugBreak();
				}

				res=0;
				m_network->SendToUser(info->index, eGNPMSG_REQ_REPLAY_OK, &res, sizeof(res));
				goid_t goid=m_goban->AddForPlayBySimpleGoData(CUserManager::GetInstance()->GetMyId(), *uid, godata);
				info->goid=goid;
				sGoPlayInfo* s = (sGoPlayInfo*)m_goban->GetExtraData(goid);
				s->hUser=(void*)info;

			}
			break;
		}
		case eGCID_REQ_REPLAY_OK:
		{
			bit32_t* buffer = (bit32_t*)param1;
			sGoUserInfo* info = (sGoUserInfo*)buffer[0];
			bit32_t res = buffer[2];
			ubit32_t myId = CUserManager::GetInstance()->GetMyId();

			if (res==0)
			{
				sSimpleGoHeader* data = CUserManager::GetInstance()->GetWaitGoHeader(info->id);
				ugoid_t uid;
				//uid=;
				uid.set(cglsGoban::MakeUniversalGoId(myId, info->id, (sSimpleGoHeader*)data));
				int size;
				sSimpleGoData* godata=CUserManager::GetInstance()->GetGoData(uid, size, True);
				
				goid_t id = m_goban->AddForPlayBySimpleGoData(myId, uid, godata);

				info->goid=id;
				sGoPlayInfo* s = (sGoPlayInfo*)m_goban->GetExtraData(id);
				s->hUser=(void*)info;
				break;
			}
			else if (res==1 ||
					res==0xFFFFFFFF)
			{
				cglsMessageBox msg;
				wchar_t wszMsg[128];
				wchar_t wszUserId[16];
				wsprintfW(wszMsg, L"%sさんから対局申し込みが断られました。", yiToUnicode(wszUserId, info->szName, 0));
				int res = msg.Show(m_main, wszMsg, L"お知らせ", MB_OK, __FONT_MESSAGEBOX__);
			}

			yifree(buffer);
			break;
		}
		case eGCID_REQ_PLAY_DIALOG:
		{
			sSimpleGoHeader* data = CUserManager::GetInstance()->GetWaitGoHeader(param1, 0);

			if (data!=0)
			{
				cglsMessageBox msg;
				wchar_t wszMsg[128];

				// この場合は打ち掛け碁
				if (((data->result>>6)&0x1FF)!=0)
				{
					const char* pszUserName = CUserManager::GetInstance()->GetUserName(param1);
					wchar_t wszUnicode[16];
					wsprintfW(wszMsg, L"%sさんと打ち掛け碁が残っていますが、対局しますか？", yiToUnicode(wszUnicode, pszUserName, 0));
				}
				// 残りは予定碁
				else
				{
				}

				int res = msg.Show(m_main, wszMsg, L"お知らせ", MB_OKCANCEL, __FONT_MESSAGEBOX__);

				if (res==IDOK)
				{
					return OnCommand(eGCID_REQ_REPLAY, 0, (IntPtr)data);
				}
			}

			return OnReqPlayDialog(from, param1, 0xFFFFFFFF);

			break;
		}
		case eGCID_REQ_PLAY:
		case eGCID_RPL_PLAY:
		{
			xTRACE("RECV eGCID_REQ_PLAY or eGCID_RPL_PLAY !!");
			float fontSize=20.0*m_goban->GetScale();
			fontSize=fontSize<11.0?11.0:fontSize;
			unsigned int* buffer=0;//=(unsigned int*)param1;
			unsigned int con1=0;//=buffer[2];
			unsigned int con2=0;//=buffer[3];
			HandleUser hUser = 0;

			if (param2!=0xFFFFFFFF)
			{
				buffer=(unsigned int*)param1;
				con1=buffer[2];
				con2=buffer[3];
				hUser=(HandleUser)buffer[0];
			}
			else
			{
				hUser = (HandleUser)CUserManager::GetInstance()->GetUserHandle(param1);
			}

			xTRACE("hUser:%x, con1:%x, con2:%x", hUser, con1, con2);

			sGoUserInfo* info = cypnGoNetwork::GetGoUserInfo(hUser);
			Bool bIsReply = (nSystemId==eGCID_REQ_PLAY)?False:True;
			cyigUserDialog user;
			int res=0;

			if (con1==0xFFFFFFFF)
			{
				wchar_t buffer[128];
				cglsMessageBox msg;
				wchar_t wszUserName[16];

				yiToUnicode(wszUserName, info->szName, 0);
				wsprintfW(buffer, L"%sさんへの対局申し込みは断られました。", wszUserName);
				msg.Show(m_main, buffer, L"お知らせ", MB_OK, &Font(L"Meiryo", 12.0));
				yifree((void*)param1);
				break;
			}

			xTRACE("Show UserDialog...... (%s)", info->szName);
			if ((res=user.Show(m_main, info->szName, con1, con2, 0, &Font(L"Meiryo", fontSize)))==0)
			{
				// この場合はキャンセルの返事を投げる。
				m_network->SendReplyPlay(hUser, 0xFFFFFFFF, 0xFFFFFFFF);

				if (buffer)
					yifree(buffer);
				break;
			}
			else if (res==1)
			{
				sGoPlayInfo* s = (sGoPlayInfo*)yialloci(sizeof(sGoPlayInfo));
				
				goid_t id = m_goban->ReserveId();

				unsigned int blackId = IAM_BLACK(con1)!=0?m_network->m_myinfo.id:info->id;
				unsigned int whiteId = IAM_BLACK(con1)==0?m_network->m_myinfo.id:info->id;

				s->timeStart = cyiTime::CurrentmSec();
				s->uid=ugoid_t(blackId, whiteId);
				s->condition1=con1;
				s->condition2=con2;
				s->hUser=(void*)hUser;
				m_network->SendRequestPlayStart(hUser, &s->condition1, con2, s->uid);

				char* pszBlackName = IAM_BLACK(s->condition1)==True?m_szMyId: info->szName;
				char* pszWhiteName = IAM_BLACK(s->condition1)==False?m_szMyId: info->szName;

				m_goban->AddForPlay(id, s, pszBlackName ,pszWhiteName, "", s->condition1, s->condition2, 0, &s->uid); 
				
				Bool bIsStartBlack=True;

				if (GET_INIT_STONE(s->condition1)>1)
				{
					bIsStartBlack=False;
				}

				if (bIsStartBlack==True)
				{
					s->black.startTime=cyiTime::CurrentmSec();
				}
				else
				{
					s->white.startTime=cyiTime::CurrentmSec();
				}

				cyiList* list = (cyiList*)CUserManager::GetInstance()->GetUserHandle(0);

				if (list &&
						list->count()!=0)
				{
					sSimpleGoHeader* header=(sSimpleGoHeader* )m_goban->ExportSimpleGoData(id, 0);
					m_network->BroadCastSend(list, eGNPMSG_START_PLAY, header, sizeof(sSimpleGoHeader));
				}
				info->goid=id;
			}
			else
			{
				// 再申し込み
				user.GetValue(con1, con2);
				xTRACE("Send requestReplyPlay..... condition:%x, condition2:%x", con1, con2);

				m_network->SendReplyPlay(hUser, con1, con2);
			}

			if (buffer)
				yifree((void*)buffer);
			break;
		}
		case eGCID_REQ_PLAY_START:
		{
			unsigned int* buffer=(unsigned int*)param1;
			sGoUserInfo* info = (sGoUserInfo*)buffer[0];
			unsigned int con1=buffer[2];
			unsigned int con2=buffer[3];
			ugoid_t  uid(buffer[4], buffer[5]);

			uid.day = buffer[6];
			uid.time = buffer[7];

			sGoPlayInfo* s = (sGoPlayInfo*)yialloci(sizeof(sGoPlayInfo));
			
			s->timeStart = cyiTime::CurrentmSec();
			s->condition1=con1;
			s->condition2=con2;

			s->uid=uid;
			s->hUser=(void*)info;
			xTRACE("from..... (%x, %x)", con1, con2);
			const char* pszBlackName = IAM_BLACK(s->condition1)==True?CUserManager::GetInstance()->GetMyName(): info->szName;
			const char* pszWhiteName = IAM_BLACK(s->condition1)==False?CUserManager::GetInstance()->GetMyName(): info->szName;

			goid_t id = m_goban->ReserveId();
			m_goban->AddForPlay(id, s, pszBlackName ,pszWhiteName, "", s->condition1, s->condition2, 0, &s->uid); 

			Bool bIsStartBlack=True;

			if (GET_INIT_STONE(s->condition1)>1)
			{
				bIsStartBlack=False;
			}

			if (bIsStartBlack==True)
			{
				s->black.startTime=cyiTime::CurrentmSec();
			}
			else
			{
				s->white.startTime=cyiTime::CurrentmSec();
			}


			cyiList* list = (cyiList*)CUserManager::GetInstance()->GetUserHandle(0);

			if (list &&
					list->count()!=0)
			{
				sSimpleGoHeader* header=(sSimpleGoHeader* )m_goban->ExportSimpleGoData(id, 0);
				m_network->BroadCastSend(list, eGNPMSG_START_PLAY, header, sizeof(sSimpleGoHeader));
				yifree(header);
			}

			//m_goban->AddTimer();
			info->goid=id;

			yifree(buffer);
			break;
		}
		case eGCID_START_PLAY:
		{
			unsigned int* buffer=(unsigned int*)param1;
			sGoUserInfo* info=(sGoUserInfo*)buffer[0];
			sSimpleGoHeader* hd = (sSimpleGoHeader*)&buffer[2];

			CUserManager::GetInstance()->AddPlayingGoHeader(info->id, hd);
			m_goban->UpdateMyView(1, info->id, 0, eMFVU_GODATA);
			yifree(buffer);
			break;
		}
		//case eGCID_START_WATCH:
		//{
		//	unsigned int id = param1;
		//	sSimpleGoHeader* header = (sSimpleGoHeader*)param2;

		//	//m_goban->AddByWatch();
		//	break;
		//}
		case eGCID_GO_RESULT:
		{
			unsigned int* s = (unsigned int*)param1;
			sGoUserInfo* info = (sGoUserInfo*)s[0];

			ugoid_t uid(s[2], s[3]);
			uid.day=s[4];
			uid.time=s[5];

			goid_t goid = m_goban->FindGoban(uid);

			if (goid)
			{
				if (m_goban->GetInputMode(goid)==eIM_WATCH)
				{
					m_goban->SetPlayResult(goid, (s[6]>>29)&0x03, s[6]&0xFF, True);
				}
			}

			sSimpleGoHeader* header=(sSimpleGoHeader* )CUserManager::GetInstance()->GetPlayingGoHeader(info->id, &uid);
			

			if (header)
			{
				header->result|=(s[6]&0x60000000);
				header->result|=((s[6]&0xFF)<<19);

				if (s[6]!=0)
				{
					CUserManager::GetInstance()->AppendGoHeader(info->id, header, 1);
				}

				CUserManager::GetInstance()->DeletePlayingGoHeader(info->id, uid);

				m_goban->UpdateMyView(1, uid.b_id, 0, eMFVU_GODATA);
				m_goban->UpdateMyView(1, uid.w_id, 0, eMFVU_GODATA);
			}



			break;
		}
		case eGCID_NOTIFY_PUTSTONE:
		{
			unsigned int* s = (unsigned int*)param1;
			sGoUserInfo* info = (sGoUserInfo*)s[0];

			ugoid_t uid(s[2], s[3]);
			uid.day=s[4];
			uid.time=s[5];
			sSimpleGoHeader* hd = (sSimpleGoHeader*)CUserManager::GetInstance()->GetPlayingGoHeader(info->id, &uid);//SearchPlayingGoHeader(uid);
			goid_t goid = m_goban->FindGoban(uid);


			if (hd)
			{
				hd->result&=(~(0x1FF<<6));
				hd->result|=((0x1FF&s[6])<<6);

				if (((hd->result>>29)&0x03)==0)
				{
					m_goban->UpdateMyView(1, uid.b_id, 0, eMFVU_GODATA);
					m_goban->UpdateMyView(1, uid.w_id, 0, eMFVU_GODATA);
				}


				if (goid!=0)
				{
					if (m_goban->GetInputMode(goid)==eIM_WATCH)
					{
						m_goban->PutStone(goid, s[7], s[8]);
						//m_goban->ChangeTimer(goid, m_goban->IsCurrentBlack(), s[8], m_goban->GetExtraData(goid));
					}
				}
			}
			yifree(s);

			break;
		}
		case eGCID_SEND_STONE:
		{
			if (from==m_goban)
			{
				stone_t stone = param1;
				cyiGoEngine* eng = (cyiGoEngine* )param2;
				sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();
				unsigned int usedTime=((STONE_TYPE(stone)==eST_BLACK)?info->black.usedTime:info->white.usedTime);

				if (info==0 ||
					info->hUser==0)
				{
					break;
				}

				if (m_network!=0)
				{
					if (info!=0 &&
						info->hUser!=0)
					{
						m_network->SendStone((HandleUser)info->hUser, stone, usedTime);
					}
				}


				if (eng->GetInputMode()==eIM_STUDY &&
						m_goban->ImBlack(eng->m_id)==False)
				{
					return 0;
				}

				cyiList* list = (cyiList*)CUserManager::GetInstance()->GetUserHandle(0);
				if (list &&
						list->count()!=0)
				{
					sGeneralParam param = {eng->m_ugoid.b_id, eng->m_ugoid.w_id, eng->m_ugoid.day, eng->m_ugoid.time, eng->GetCurrentOrder(), eng->GetLastStone(), usedTime, 0};
					m_network->BroadCastSend(list, eGNPMSG_NOTIFY_PUTSTONE, &param, GP_SIZE(7));
				}

				return 0;
			}
			else
			{
				unsigned int* buffer=(unsigned int*)param1;
				HandleUser hUser = (HandleUser)buffer[0];
				sGoUserInfo* info = (sGoUserInfo*)hUser;
				xTRACE("eGCID_SEND_STONE recevied used time:%d", buffer[3]);
				m_goban->PutStone(info->goid, buffer[2], buffer[3], True);

				cyiList* list = (cyiList*)CUserManager::GetInstance()->GetUserHandle(0);
				if (list &&
						list->count()!=0)
				{
					cyiGoEngine* eng = m_goban->GetGoEngineById(info->goid);
					sGeneralParam param = {eng->m_ugoid.b_id, eng->m_ugoid.w_id, eng->m_ugoid.day, eng->m_ugoid.time, eng->GetCurrentOrder(), eng->GetLastStone(), buffer[3], 0};
					m_network->BroadCastSend(list, eGNPMSG_NOTIFY_PUTSTONE, &param, GP_SIZE(7));
				}

				yifree(buffer);

				break;
			}
			break;
		}
		case eGCID_SEND_CMD:
		{
			if (from==0)
			{
				unsigned int cmd = param1;
				cyiGoEngine* eng = (cyiGoEngine* )m_goban->GetGoEngineById(param2);
				sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();

				xTRACE("OnCommand(eGCID_SEND_CMD) %d from me %d", cmd, m_goban->ImBlack());

				if (m_network!=0)
					m_network->SendToUserEx((HandleUser)info->hUser, eGNPMSG_SEND_CMD, &cmd, sizeof(cmd));

				return 0;
			}
			else
			{
				unsigned int* buffer=(unsigned int*)param1;
				HandleUser hUser = (HandleUser)buffer[0];
				sGoUserInfo* info = (sGoUserInfo*)hUser;
				xTRACE("OnCommand(eGCID_SEND_CMD) %d from network", buffer[2], m_goban->ImBlack());

				OnCommand(buffer[2], 0, info->goid);
				//m_goban->PutStone(info->goid, buffer[2], buffer[3], True);

				yifree(buffer);

				break;
			}
			break;
		}
		case eGCID_CONNECTION_CLOSED:
		{
			sGoUserInfo* info = (sGoUserInfo*)param1;
			cglsToolContainer* con = m_goban->GetContainer();

			char temp[1024];
			sMsg* msg = (sMsg*)temp;
			wchar_t buffer[64];
			wchar_t pwszUserName[16];
			
			yiToUnicode(pwszUserName, info->szName,0);
			wsprintfW((wchar_t*)buffer, L"%sさんとの接続が切れました。", pwszUserName);
			MakeMessage(msg, 0, info->id, info->szName, buffer);
			con->AddMessage(msg, True);
			CUserManager::GetInstance()->RemoveUserHandle(info->id);
			CUserManager::GetInstance()->DeletePlayingGoHeader(info->id, -1);
			
			m_goban->UpdateMyView(1, 0, 0, 0xFF);
			//yifree(info);
			
			if (info->goid!=0)
			{
				StopTimer(info->goid);
				sGoPlayInfo* s = (sGoPlayInfo* )m_goban->GetExtraData(info->goid);
				s->hUser=(void*)info->id;
				m_goban->ChangeInputMode(info->goid, eIM_WAIT_USER);
			}
			
			int remoteId=info->id;
			m_network->DestroyUser((HandleUser)info);

			//sypnUserInfo remote;
			//if (m_network->Connect(remoteId, 0, 0, 0, &remote)<0)
			//{
			//	break;
			//}
			break;
		}
		case eGCID_WATCH_GO:
		{
			if (from!=m_main)
			{
				unsigned int userId = param1;
				sSimpleGoHeader* p = (sSimpleGoHeader*)param2;
				ugoid_t uid(userId, p->remoteId);

				HandleUser hUser = (HandleUser)CUserManager::GetInstance()->GetUserHandle(userId);

				if (hUser==0)
					break;

				if ((p->result&0x80000000)==0)
				{
					uid.b_id=p->remoteId;
					uid.w_id=userId;
				}

				uid.day=p->day;
				uid.time=p->time;
				
				m_network->SendToUserEx(hUser, eGNPMSG_WATCH_GO, &uid, sizeof(uid), True);
				break;
			}
			else
			{
				unsigned int* buffer=(unsigned int*)param1;
				ugoid_t* uid=(ugoid_t*)&buffer[2];
				int size;

				goid_t goid = m_goban->FindGoban(*uid);

				if (goid==0)
				{
					sGeneralParam param = {0, };
					m_network->SendToUserEx((HandleUser)buffer[0], eGNPMSG_WATCH_GO_RESULT, &param, sizeof(unsigned int), True);
					yifree(buffer);
					break;
				}

				sSimpleGoData* data = m_goban->ExportSimpleGoData(goid, &size);

				if (data==0)
				{
					sGeneralParam param = {0, };
					m_network->SendToUserEx((HandleUser)buffer[0], eGNPMSG_WATCH_GO_RESULT, &param, sizeof(unsigned int), True);
				}
				else
				{
					int sendByte = size>(512-16)?(512-16):size;

					if (m_network->SendToUserEx((HandleUser)buffer[0], eGNPMSG_WATCH_GO_RESULT, data, size, False)==False)
					{
						yifree(buffer);
						break;
					}

					if (size-sendByte>0)
					{
						char* pIndex=(char*)data;
						char xbuffer[512];
						int remain=size-sendByte;

						memcpy(xbuffer, pIndex, sizeof(sSimpleGoHeader));
						memcpy(&xbuffer[sizeof(sSimpleGoHeader)], &pIndex[sendByte], remain);

						sSimpleGoData* xxxx = (sSimpleGoData*)xbuffer[sizeof(sSimpleGoHeader)];

						if (m_network->SendToUserEx((HandleUser)buffer[0], eGNPMSG_WATCH_GO_RESULT, xbuffer, remain+sizeof(sSimpleGoHeader), False)==False)
						{
							yifree(buffer);
							break;
						}
					}

					sGeneralParam param;
					memcpy(&param, uid, sizeof(*uid));
					param.param[4]=m_goban->GetTotalUsedTime(goid, True);//m_goban->GetTotalUsedTime(goid, True);
					param.param[5]=m_goban->GetTotalUsedTime(goid, False);

					m_network->SendToUserEx((HandleUser)buffer[0], eGNPMSG_WATCH_REMAIN_TIME, &param, GP_SIZE(6), False);//==False)
				}
				yifree(buffer);
				break;
			}

			break;
		}
		case eGCID_WATCH_REMAIN_TIME:
		{
			unsigned int* xxxx = (unsigned int*)param1;
			sGoUserInfo* info = (sGoUserInfo*)xxxx[0];
			ugoid_t* uid = (ugoid_t*)&xxxx[2];

			goid_t goid=m_goban->FindGoban(*uid);

			if (goid)
			{
				m_goban->SetTotalRemainTime(goid, xxxx[6], True);
				m_goban->SetTotalRemainTime(goid, xxxx[7], False);
			}

			break;
		}
		case eGCID_WATCH_GO_RESULT:
		{
			unsigned int* buffer=(unsigned int*)param1;
			sGoUserInfo* info = (sGoUserInfo*)buffer[0];
			
			if (buffer[1]==4)
			{
				yifree(buffer);
				break;
			}

			sSimpleGoHeader* hd = (sSimpleGoHeader*)&buffer[2];
			int size = buffer[1];
			
			ugoid_t uid(info->id, hd->remoteId);

			if ((hd->result&0x80000000)==0)
			{
				uid.w_id=info->id;
				uid.b_id=hd->remoteId;
			}

			uid.day=hd->day;
			uid.time=hd->time;

			goid_t id = m_goban->FindGoban(uid);

			if (id==0)
			{
				const char* pszBlackName = CUserManager::GetInstance()->GetUserName(uid.b_id);
				const char* pszWhiteName = CUserManager::GetInstance()->GetUserName(uid.w_id);
				m_goban->AddForWatch((sSimpleGoData*)hd, size, uid, pszBlackName, pszWhiteName);
				yifree(buffer);

				break;
			}

			int n = (size-sizeof(sGoHeader))>>1;
			unsigned short* pStone = (unsigned short*)&hd[1];

			for (int i=0; i<n; i++)
			{
				eStoneType type=eST_NONE;
				if (((pStone[i]>>10)&1)==1)
				{
					if (((pStone[i]>>11)&1)==1)
					{
						type=eST_BPASS;
					}
					else
					{
						type=eST_BLACK;
					}
				}
				else
				{
					if (((pStone[i]>>11)&1)==1)
					{
						type=eST_WPASS;
					}
					else
					{
						type=eST_WHITE;
					}
				}
				m_goban->PutStone(id, MAKESTONE(type, 0, (pStone[i]&0x1F), (pStone[i]>>5)&0x1F), 0);
			}
			
			break;
		}
		//case eGCID_STONE_TEXT:
		//{
		//	if (from==m_goban)
		//	{
		//		stone_t stone		= param1;
		//		wchar_t* message	= (wchar_t*)param2;
		//		int len=0;
		//		if (message==0 ||
		//				message[0]==0)
		//				break;

		//		sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);
		//		len = wcslen(message);

		//		int sendSize=(len<<1)+sizeof(stone_t)+2;
		//		unsigned char* pBuffer=yialloc(sendSize);
		//		memcpy(pBuffer, &stone, sizeof(stone));
		//		wcscpy(&pBuffer[sizeof(stone)], message);
		//		
		//		m_network->SendToUserEx((HandleUser)info->hUser, eGNPMSG_STONE_TEXT, pBuffer, sendSize);

		//		wsprintfW(buffer, L"この手(%c,%c)は、%S", STONE_POS_X(stone)+'a', STONE_POS_Y(stone)+'A', );
		//		m_goban->AppendMessage(info->goid, pszText, wcslen(pszText));
		//		//yiToMultiByteCode(&pBuffer[sizeof(stone)], message, 0);


		//	}
		//	else
		//	{
		//		unsigned int* p = (unsigned int*)param1;
		//		sGoUserInfo* info = (sGoUserInfo* )p[0];
		//		stone_t stone = (stone_t*)p[2];
		//		wchar_t* pszText = (wchar_t*)&p[3];
		//		int x = STONE_POS_X(stone);
		//		int y = STONE_POS_Y(stone);
		//		m_goban->ShowBaloon(stone, pszText, True);
		//		m_goban->AppendMessage(info->goid, pszText, wcslen(pszText));
		//	}
		//	break;
		//}
		case eGCID_TIME_LOSE:
		{
			if ((param1&0xFFFFFF00)==0)
			//if (from!=m_main)
			{
				unsigned int remoteId=0;
				xTRACE("CGoEditorApp::OnCommand(eGCID_TIME_LOSE) %d, %d", param1, param2);
				if (m_goban->ImBlack(param2)!=param1)
				{
					xTRACE("CGoEditorApp::OnCommand(eGCID_TIME_LOSE) exit");
					break;
				}

				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(param2);
				HandleUser hRemoteUser =info->hUser;
				remoteId = ((sGoUserInfo*)hRemoteUser)->id;
				if (CUserManager::GetInstance()->IsMyFriend(remoteId)==True ||
						CUserManager::GetInstance()->IsGroupUser(remoteId)==True )
				{
					m_goban->ChangeInputMode(eIM_STUDY);
				}
				else
				{
					m_goban->ChangeInputMode(eIM_ENJOY);
					sGoUserInfo* goInfo=(sGoUserInfo*)info->hUser;
					goInfo->goid=0;
					info->hUser=0;
				}

				if (m_network)
				{
					goid_t id = param2;//m_goban->GetCurrentGoId();
					sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(id);
					m_network->SendTimeout((HandleUser)info->hUser, param1, param2);
				}

				//wchar_t buffer[128];
				//wsprintfW(buffer, L"%s\r\n%s勝ち", L"時間切れ", m_goban->ImBlack(param2)?L"白":L"黒");
				//cglsMessageBox msg;// = new cglsMessageBox();
				//msg.Show(m_main, buffer, L"エラー", MB_OK, &Font(L"Meiryo", 13.0));

				unsigned int day=0;
				unsigned int time = cyiTime::GetLocalTime(day);
				char szNewFileName[256];
				wsprintf(szNewFileName, "%d%02d%02d%02d%02d%02d.sgf", DAY_YEAR(day), DAY_MON(day), DAY_DAY(day),
															xHOUR(time), xMINUTE(time), xSECOND(time));

				
				int result=(((param1==1?2:1)<<29)|0xFE);
				
				xRemovePlayingGo(remoteId, *m_goban->GetUniversalGoid(param2), True);
				SendBroadCast(*m_goban->GetUniversalGoid(param2), eGNPMSG_GO_RESULT, &result, sizeof(result));

				m_goban->SetPlayResult(0, param1==1?2:1, 0xFE, True);
				//m_goban->SetPlayResult(0, ((1-m_goban->ImBlack(param2))==1)?2:1, 0xFE, True);

				SaveGoDataEx(param2);

				//m_goban->GetHeaderEx(0, &hd);

				//char path[256];
				//
				//if (m_network->m_myinfo.id)
				//{
				//	wsprintf(path, "%s\\data\\other\\%s", m_szMyDir, szNewFileName);
				//}
				//else
				//{
				//	wsprintf(path, "%s\\data\\other\\%s", m_szUserDir, szNewFileName);
				//}

				//m_goban->Save(path);
				//SaveGoData(1, &hd.header, szNewFileName, hd.map, hd.n);

				break;
			}
			else // from network.
			{
				unsigned int* received=(unsigned int*)param1;
				sGoUserInfo* info = (sGoUserInfo*)received[0];
				xTRACE("CGoEditorApp::OnCommand(eGCID_TIME_LOSE) recived from (%d, %d)", received[2], received[3]);

				unsigned int id = info->id;//((sGoUserInfo*)hRemoteUser)->id;
				if (CUserManager::GetInstance()->IsMyFriend(id)==True ||
						CUserManager::GetInstance()->IsGroupUser(id)==True )
				{
					m_goban->ChangeInputMode(info->goid,eIM_STUDY);
				}
				else
				{
					m_goban->ChangeInputMode(info->goid,eIM_ENJOY);
					sGoPlayInfo* goinfo = (sGoPlayInfo*)m_goban->GetExtraData(info->goid);
				}

				//m_goban->ChangeInputMode(info->goid, eIM_ENJOY);
				//wchar_t buffer[128];
				//wsprintfW(buffer, L"%s\r\n%s勝ち", L"時間切れ", received[2]?L"白":L"黒");
				//cglsMessageBox msg;// = new cglsMessageBox();
				//msg.Show(m_main, buffer, L"エラー", MB_OK, &Font(L"Meiryo", 13.0));

				int result=(((received[3]==1?2:1)<<29)|0xFE);
				SendBroadCast(*m_goban->GetUniversalGoid(info->goid), eGNPMSG_GO_RESULT, &result, sizeof(result));

				xRemovePlayingGo(info->id, *m_goban->GetUniversalGoid(info->goid), False);
				m_goban->SetPlayResult(info->goid, received[3]?2:1, 0xFE, True);
				SaveGoDataEx(info->goid);

				info->goid=0;
				
				yifree(received);
				//m_goban->UpdateMyView(0, 0, 0, );

				//SaveGoData(1, &hd.header, szNewFileName, hd.map, hd.n);
				break;
			}
		}
		case eGCID_GIVEUP:
		{
			if (from!=m_main)
			{
				cglsMessageBox msg;
				if (msg.Show(m_main, L"本当に投了しますか？", L"エラー", MB_OKCANCEL, &Font(L"Meiryo", 13.0))==True)
				{
					if (m_network)
					{
						goid_t id = m_goban->GetCurrentGoId();
						sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(id);
						m_network->SendGiveup((HandleUser)info->hUser);

						sGoUserInfo* s = (sGoUserInfo* )info->hUser;
						if (CUserManager::GetInstance()->IsMyFriend(s->id)==True ||
							CUserManager::GetInstance()->IsGroupUser(s->id)==True )
						{
							m_goban->ChangeInputMode(eIM_STUDY);
						}
						else
						{
							m_goban->ChangeInputMode(eIM_ENJOY);
							sGoUserInfo* goInfo=(sGoUserInfo*)info->hUser;
							goInfo->goid=0;
							info->hUser=0;
						}

						
						int result=(((m_goban->ImBlack()?2:1)<<29)|0xFF);
						xRemovePlayingGo(s->id, *m_goban->GetUniversalGoid(id), True);
						SendBroadCast(*m_goban->GetUniversalGoid(id), eGNPMSG_GO_RESULT, &result, sizeof(result));
					}
					else
					{
						m_goban->ChangeInputMode(eIM_ENJOY);

					}

					unsigned int day=0;
					unsigned int time = cyiTime::GetLocalTime(day);
					char szNewFileName[256];

					
					sGoHeaderEx hd;
					m_goban->SetPlayResult(0, (m_goban->ImBlack())?2:1, 0xFF, True);
					SaveGoDataEx(m_goban->GetCurrentGoId());
					break;
				}
			}
			else
			{
				int index = param1;
				sGoUserInfo* info = (sGoUserInfo* )m_network->IndexToUser(index);
				if (CUserManager::GetInstance()->IsMyFriend(info->id)==True ||
					CUserManager::GetInstance()->IsGroupUser(info->id)==True )
				{
					m_goban->ChangeInputMode(info->goid, eIM_STUDY);
				}
				else
				{
					m_goban->ChangeInputMode(info->goid, eIM_ENJOY);
					sGoPlayInfo* pInfo = (sGoPlayInfo*)m_goban->GetExtraData(info->goid);
					pInfo->hUser=0;
				}
				
				unsigned int day=0;
				unsigned int time = cyiTime::GetLocalTime(day);
				char szNewFileName[256];
				wsprintf(szNewFileName, "%d%02d%02d%02d%02d%02d.sgf", DAY_YEAR(day), DAY_MON(day), DAY_DAY(day),
															xHOUR(time), xMINUTE(time), xSECOND(time));

				sGoHeaderEx hd;
				m_goban->SetPlayResult(0, m_goban->ImBlack(info->goid)?1:2, 0xFF, True);
				
				SaveGoDataEx(info->goid);

				int result=(((m_goban->ImBlack(info->goid)?2:1)<<29)|0xFF);
				xRemovePlayingGo(info->id, *m_goban->GetUniversalGoid(info->goid), False);
				SendBroadCast(*m_goban->GetUniversalGoid(info->goid), eGNPMSG_GO_RESULT, &result, sizeof(result));

				info->goid=0;

			}
			break;
		}
		case eGCID_PAUSE:
		{
			if (from==m_main)
			{
				sGoUserInfo* s = (sGoUserInfo*)m_network->IndexToUser(param1);
				m_goban->ChangeInputMode(s->goid, eIM_EDIT);
				sGoPlayInfo* info = (sGoPlayInfo* )m_goban->GetExtraData(s->goid);


				int size;
				sSimpleGoData* data = m_goban->ExportSimpleGoData(s->goid, 0);
				m_network->SendToServer(eUSPID_UP_GODATA2 , data, sizeof(sSimpleGoHeader), False);

				CUserManager::GetInstance()->AddWaitGoHeader((sSimpleGoHeader*)data);

				yifree(data);

				int result=0;
				SendBroadCast(*m_goban->GetUniversalGoid(s->goid), eGNPMSG_GO_RESULT, &result, sizeof(result));

				m_goban->ShowMessage(s->goid, L"打ちかけになりました。", L"", MB_OK);

				info->hUser=0;
				s->goid=0;
				

			}
			else
			{
				// 押した方がサーバへ保存。
				// それでも黒の方のディレクトリに保存されます。
				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);

				m_network->SendToUserEx(info->hUser, eGNPMSG_PAUSE, 0, 0);
				m_system->EnableCommand(eGNPMSG_PAUSE, False);


				int size=0;
				sSimpleGoData* data = m_goban->ExportSimpleGoData(0, &size);

				int curr=0, cnt=0;
				m_goban->GetPlayTime(0, &curr, &cnt, True);
				data->reserved[0]=(curr<<16)|(cnt);
				m_goban->GetPlayTime(0, &curr, &cnt, False);
				data->reserved[1]=(curr<<16)|(cnt);
				m_network->SendToServer(eUSPID_UP_GODATA2 ,data, size, False);
				CUserManager::GetInstance()->AddWaitGoHeader((sSimpleGoHeader*)data);

				yifree(data);

				sGoUserInfo* s = (sGoUserInfo*)info->hUser;

				int result=0;
				SendBroadCast(*m_goban->GetUniversalGoid(s->goid), eGNPMSG_GO_RESULT, &result, sizeof(result));

				s->goid=0;
				info->hUser=0;
				m_goban->ChangeInputMode(eIM_EDIT);
			}
			break;
		}
		case eGCID_CANCEL:
		{
			if (from==m_goban) // メッセージボックスの返事
			{
				if (param1==IDOK)
				{
					m_goban->ChangeInputMode(eIM_EDIT);
					sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);
					sGoUserInfo* s=(sGoUserInfo*)info->hUser;

					xRemovePlayingGo(s->id, *m_goban->GetUniversalGoid(0), True);
					m_network->SendToUserEx(info->hUser, eGNPMSG_CANCEL_FORCED, 0, 0);

					int result=0;
					SendBroadCast(*m_goban->GetUniversalGoid(0), eGNPMSG_GO_RESULT, &result, sizeof(result));


					s->goid=0;
					info->hUser=0;
				}
				else
				{
					m_goban->ChangeInputMode(eIM_EDIT);
					sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);
					m_network->SendToUserEx(info->hUser, eGNPMSG_NO_CANCEL, 0, 0);
					m_goban->StartTimer(0);
				}
				break;
			}
			else if (from!=m_main)
			{
				int n = m_goban->GetCurrentGoEngine()->GetCurrentOrder();
				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);

				if (n<15)
				{
					m_goban->ChangeInputMode(eIM_EDIT);
					m_network->SendToUserEx(info->hUser, eGNPMSG_CANCEL_FORCED, 0, 0);

					int result=0;
					SendBroadCast(*m_goban->GetUniversalGoid(0), eGNPMSG_GO_RESULT, &result, sizeof(result));

					sGoUserInfo* s=(sGoUserInfo*)info->hUser;
					s->goid=0;
					info->hUser=0;

					break;
				}

				m_network->SendToUserEx(info->hUser, eGNPMSG_CANCEL, 0, 0);
				m_system->EnableCommand(eGCID_CANCEL, False);
			}
			else
			{
				sGoUserInfo* info = (sGoUserInfo*)m_network->IndexToUser(param1);
				StopTimer(info->goid);
				m_goban->ShowMessage(info->goid, L"対局中断を申し込まれました。対局を中断しますか？", L"対局中断", MB_OKCANCEL, eGCID_CANCEL);
			}
			break;
		}
		case eGCID_CANCEL_FORCED:
		{
			if (from==m_main)
			{
				sGoUserInfo* info = (sGoUserInfo*)m_network->IndexToUser(param1);

				m_goban->ChangeInputMode(eIM_EDIT);

				int result=0;
				xRemovePlayingGo(info->id, *m_goban->GetUniversalGoid(info->goid), False);
				SendBroadCast(*m_goban->GetUniversalGoid(info->goid), eGNPMSG_GO_RESULT, &result, sizeof(result));

				m_goban->ShowMessage(info->goid, L"対局が取り消されました。", 0, MB_OK);
				m_system->EnableCommand(eGCID_CANCEL, True);

				sGoPlayInfo* s = (sGoPlayInfo*)m_goban->GetExtraData(info->goid);
				s->hUser=0;
				info->goid=0;

				break;
			}
			else
			{

				m_system->EnableCommand(eGCID_CANCEL, True);
				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);

				if (param1==1)
				{
					m_goban->ChangeInputMode(eIM_EDIT);
					m_network->SendToUserEx(info->hUser, eGNPMSG_CANCEL_FORCED, 0, 0);
					sGoUserInfo* s=(sGoUserInfo*)info->hUser;

					int result=0;
					xRemovePlayingGo(s->id, *m_goban->GetUniversalGoid(0), True);
					SendBroadCast(*m_goban->GetUniversalGoid(0), eGNPMSG_GO_RESULT, &result, sizeof(result));

					s->goid=0;
					info->hUser=0;
				}
				else
				{
					m_network->SendToUserEx(info->hUser, eGNPMSG_NO_CANCEL, 0, 0);
				}
			}
			break;
		}
		case eGCID_NO_CANCEL:
		{
			sGoUserInfo* info = (sGoUserInfo*)m_network->IndexToUser(param1);
			m_system->EnableCommand(eGCID_CANCEL, True);
			m_goban->StartTimer(info->goid);
			m_goban->ShowMessage(info->goid, L"対局中断がキャンセルされました。", 0, MB_OK);
			break;
		}
		case eGCID_SECOND_READ:
		{
			xTRACE("%d 秒読み、残り%d秒", param1, param2);
			if (m_goban->ImBlack(param1)==m_goban->IsCurrentBlack(param1))
			{
				m_goban->SetSecondRead(param1, param2);
			}
			break;
		}
		// リストから入れたり外すため。
		case eGCID_ADD_USER:
		{
			int index = param1;
			HandleUser hUser=m_network->IndexToUser(param1);
			if (hUser==0)
				break;

			sGoUserInfo* p = (sGoUserInfo*)hUser;

			xTRACE("CGoEditorApp::OnCommand(eGCID_ADD_USER)");

			if (CUserManager::GetInstance()->GetUserHandle(p->id)==0)
			{
				CUserManager::GetInstance()->AddUserHandle((Handle)hUser);//m_network->IndexToUser(ypnUser.index));
			}
			else // 既に登録済みの場合。
			{
				
			}

			if (m_network->SendToUser(index, (int)eGNPMSG_REQ_INFO_DETAIL, 0, 0)==False)
			{
				xTRACE("CGoEditorApp::OnCommand() eGNPMSG_REQ_INFO_DETAIL send error");
				m_network->ConnectionClose(0, index);//DisConnectUser(index);
				return 0;
			}

			if (p->id!=0)
			{
				CUserManager::GetInstance()->AddDictionary(p->id, p->szName, 0);
			}

			//m_network->GetUserInfo(param1);
			//m_userz->add(cypnGoNetwork::GetGoUserInfo(hUser));
			break;
		}
		case eGCID_REMOVE_USER:
		{
			//int index=param1;
			//m_userz->lock();
			//for (cyitNode<sGoUserInfo>* node=m_userz->GetNodeTopFrom(); node; node=node->np)
			//{
			//	sGoUserInfo* info = node->ref;

			//	if (info->index==index)
			//	{
			//		m_userz->remove(node);
			//		break;
			//	}
			//}
			//m_userz->unlock();
			//OnCommand(eGCID_UPDATE_FRIEND_VIEW, 0,0, 0);
			break;
		}

		case eGCID_REQ_INFO_DETAIL:
		{
			int index=param1;
			xTRACE("CGoEditorApp::OnCommand(eGCID_REQ_INFO_DETAIL)");

			if (CUserManager::GetInstance()->GetMyId()!=0)
			{
				if (m_network->SendToUser(index, eGNPMSG_RPY_INFO_DETAIL, m_network->GetMyGoInfo(), sizeof(sGoUserInfoEx))==False)
				{
					xTRACE("CGoEditorApp::OnCommand(eGCID_REQ_INFO_DETAIL) Failure....");
					m_network->ConnectionClose(0, index);//DisConnectUser(index);
				}
			}
			else
			{
				if (m_network->SendToUser(index, eGNPMSG_RPY_INFO_DETAIL, m_network->GetMyGoInfo(), sizeof(sGoUserInfo))==False)
				{
					xTRACE("CGoEditorApp::OnCommand(eGCID_REQ_INFO_DETAIL) Failure....");
					m_network->ConnectionClose(0, index);
				}
			}
			//	int size;
			//	sypnUserInfo* info=(sypnUserInfo*)m_network->GetUserInfo(index);
			//	void* p = CUserManager::GetInstance()->ExportUserInfo(info->id, &size);

			//	if (info==0)
			//	{
			//		DebugBreak();				
			//		break;
			//	}
			//	if (m_network->SendToUser(index, eGNPMSG_RPY_INFO_DETAIL, p, size)==False)
			//	{
			//		m_network->DisConnectUser(index);
			//	}
			//	yifree(p);
			//}
			break;
		}
		case eGCID_RPY_INFO_DETAIL:
		{
			unsigned int* p = (unsigned int*)param1;
			HandleUser hUser = (HandleUser)p[0];
			int size = p[1];
			sGoUserInfoEx* detail = (sGoUserInfoEx*)&p[2];
			sGoUserInfo* s = (sGoUserInfo*)hUser;
			
			CUserManager::GetInstance()->SetRegisterInfo(&detail->reg);
			CUserManager::GetInstance()->SetScore(s->id, detail->score);

			//unsigned int seq;
			//Bitmap* bmp = CUserManager::GetInstance()->GetPicture(s->id, seq);

			//if (m_network->SendToUser(s->index, eGNPMSG_REQ_PICTURE, &seq, sizeof(seq))==False)
			//{
			//	m_network->DisConnectUser(s->index);
			//}

			if (CUserManager::GetInstance()->IsMyFriend(s->id)==True)
			{
				m_goban->UpdateMyView(1, 0, 0, 0xFF);
			}
			
			if (CUserManager::GetInstance()->IsGroupUser(s->id)==True)
			{
				m_goban->UpdateMyView(2, s->id, 0, 0xFF);
			}
			
			// get picture file.
			unsigned int seq;

			CUserManager::GetInstance()->GetPicture(s->id, seq);
			RequestDataFrom(hUser, 1, seq);

			CUserManager::GetInstance()->GetGoHeader(s->id, &size);
			RequestDataFrom(hUser, 2, size);

			// kihu.
			//RequestDataFrom(hUser, 2);

			goid_t* ids = m_goban->GetCurrentPlayGobanList();
			
			if (ids)
			{
				for (int i=0; ids[i]!=0; i++)
				{
					sSimpleGoHeader* header=m_goban->GetSimpleGoHeader(ids[i]);
					m_network->SendToUserEx(hUser, eGNPMSG_CURRENT_GO_LIST, header, sizeof(sSimpleGoHeader));
				}

				yifree(ids);
			}

			//goid_t goid;
			//if ((goid=m_goban->GetWaitGoban(s->id))!=0)
			//{
			//	m_network->SendToUserEx(hUser, eGNPMSG_REQ_PLAY_START, data, size, True);
			//	m_goban->ChangeInputMode(goid);
			//	StartTimer(goid);
			//}

			yifree(p);

			break;
		}
		case eGCID_CURRENT_GO_LIST:
		{
			unsigned int* p = (unsigned int*)param1;
			HandleUser hUser = (HandleUser)p[0];
			sGoUserInfo* s=(sGoUserInfo*)hUser;
			sSimpleGoHeader* header = (sSimpleGoHeader* )&p[2];

			CUserManager::GetInstance()->AddPlayingGoHeader(s->id, header);
			m_goban->UpdateMyView(1, s->id, 0, eMFVU_GODATA);
			yifree(p);
			break;
		}
		case eGCID_HAGASHI:
		{
			if (from==m_main)
			{
				HandleUser hUser = (HandleUser)m_network->IndexToUser(param1);
				unsigned int goid = ((sGoUserInfo*)hUser)->goid;
				if (hUser==0)
				{
					// 接続終了？
					break;
				}

				// 自分の番でなかったら、既に打ってしまったことになる。
				if (m_goban->IsCurrentBlack()!=m_goban->ImBlack())
				{
					m_goban->DeleteNode(goid, 0);
				}
				m_goban->DeleteNode(goid, 0);
				
				int n = m_goban->GetRemainTime(0, m_goban->ImBlack()?1:0);
				m_goban->SetRemainTime(0, -2, m_goban->ImBlack()?1:0);
				m_goban->ChangeTimer(goid, m_goban->ImBlack()?0:1, 0, 0, True);
			}
			else
			{
				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);
				HandleUser hUser = info->hUser;
				m_network->SendToUser(((sGoUserInfo*)hUser)->index, eGNPMSG_HAGASHI, 0, 0);
				m_goban->DeleteNode(0);

				int n = m_goban->GetRemainTime(0, m_goban->ImBlack()?0:1);
				m_goban->SetRemainTime(0, -2, m_goban->ImBlack()?0:1); // 2秒足す。
				m_goban->ChangeTimer(0, m_goban->ImBlack()?1:0, 0, 0, True);
			}
			break;
		}
		case eGCID_WAIT:
		{
			if (from==m_main)
			{
				xTRACE("CGoEditorApp::OnCommand(eGCID_WAIT) received! ");
				HandleUser hUser = (HandleUser)m_network->IndexToUser(param1);
				//HandleUser hUser = (HandleUser)buffer[0];
				//int nUsedTime = buffer[2];
				unsigned int goid = ((sGoUserInfo*)hUser)->goid;

				if (hUser==0)
				{
					// 接続終了？
					break;
				}

				m_goban->ShowMessage(goid, L"相手から待ったを申し込まれました。\r\n応じますか？", L"", MB_OKCANCEL, eGCID_WAIT);
			}
			else if (from==m_goban)
			{
				goid_t goid=m_goban->MessageIdToGoId((param1&0xFFFF));
				int notify = param1>>16;

				if (goid==0)
					break;

				sGoPlayInfo* s = (sGoPlayInfo*)m_goban->GetExtraData(goid);
				HandleUser hUser = s->hUser;
				if (notify==IDCANCEL)
				{
					if (s!=0)
					{
						unsigned int res=0;
						m_network->SendToUserEx(s->hUser, eGNPMSG_WAIT_RESULT, &res, sizeof(res));
					}
					break;
				}


				unsigned int n = (GET_PLAY_TIME(s->condition1)*60)/3;
				unsigned int nRemain = m_goban->GetRemainTime(goid, !m_goban->ImBlack(goid));

				nRemain = (n>=nRemain)?nRemain:n;

				//nRemain = nRemain-n;
				//nRemain=nRemain<0?0:nRemain;
				unsigned int send[3] = {1, -1, nRemain};

				if (m_goban->GetRemainTime(goid, !m_goban->ImBlack()))
				{
					m_goban->SetRemainTime(goid, nRemain, !m_goban->ImBlack());
				}
				else
				{
					int n = m_goban->GetSecondReadCount(goid, !m_goban->ImBlack());
					m_goban->SetSecondReadCount(goid, n-1, !m_goban->ImBlack());
				}

				if (m_goban->IsCurrentBlack(goid)==m_goban->ImBlack())
				{
					//DebugBreak();
					m_goban->DeleteNode(goid, 0);
					//m_goban->Goto(goid, -1, 0);
				}
				else
				{
					m_goban->Goto(goid, -1, 0);
					m_goban->DeleteNode(goid, 0);
					send[1]=(unsigned int)-2;
				}

				m_network->SendToUserEx(hUser, eGNPMSG_WAIT_RESULT, &send, sizeof(send));

				m_goban->StartTimer(goid);

				//yifree(buffer);
				break;
				
			}
			else
			{
				// 1. メッセージボックスで確認する。
				cglsMessageBox msg;
				if (msg.Show(m_main, L"持ち時間が減らされますが、\r\n待ったを申し込みますか？", L"", MB_OKCANCEL, &Font(L"Meiryo", 12.0))==False)
				{
					break;
				}

				goid_t id = m_goban->GetCurrentGoId();
				StopTimer(id);

				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(id);

				xTRACE("send eGNPMSG_WAIT............");
				if (m_network->SendToUserEx((HandleUser)info->hUser, eGNPMSG_WAIT, 0, 0)==False)
				{
					// どーする？
					m_goban->StartTimer(id);
					break;
				}
				break;
			}
			break;
		}
		case eGCID_WAIT_RESULT:
		{
			xTRACE("CGoEditorApp::OnCommand(eGCID_WAIT_RESULT) received");
			unsigned int* buffer = (unsigned int*)param1;
			HandleUser hUser = (HandleUser)buffer[0];
			int	n = buffer[2];
			int id = ((sGoUserInfo*)hUser)->goid;

			if (n==0)
			{
				sGoUserInfo* info=(sGoUserInfo*)hUser;
				m_goban->ShowMessage(info->goid, L"待ったの申し込みを断られました。", L"", MB_OK);
				m_goban->StartTimer(id);
				yifree(buffer);
				break;
			}

			sGoPlayInfo* s = (sGoPlayInfo*)m_goban->GetExtraData(id);

			if ((int)buffer[3]==-1)
			{
				m_goban->DeleteNode(id, 0);
			}
			else
			{
				m_goban->Goto(id, -1, 0);
				m_goban->DeleteNode(id, 0);
			}
			//m_goban->Goto(id, (int)buffer[3], 0);
			m_goban->SetRemainTime(id, buffer[4], m_goban->ImBlack(id));
			m_goban->StartTimer(id);

			yifree(buffer);
			
			xTRACE("CGoEditorApp::OnCommand(eGCID_WAIT_RESULT) exit");
			break;
		}
		case eGCID_STUDY_END:
		{
			if (from==m_main)
			{
				sGoUserInfo* info = (sGoUserInfo*)m_network->IndexToUser(param1);
				m_goban->ChangeInputMode(info->goid, eIM_EDIT);
				m_system->EnableCommand(eGCID_PASS, True);				

				sGoPlayInfo* pInfo = (sGoPlayInfo*)m_goban->GetExtraData(info->goid);
				pInfo->hUser=0;
				info->goid=0;
			}
			else
			{
				sGoPlayInfo* info = (sGoPlayInfo*)m_goban->GetExtraData(0);
				m_goban->ChangeInputMode(eIM_EDIT);
				m_network->SendToUserEx(info->hUser, eGNPMSG_STUDY_END, 0, 0);
				m_system->EnableCommand(eGCID_PASS, True);

				sGoUserInfo* goInfo=(sGoUserInfo*)info->hUser;
				goInfo->goid=0;
				info->hUser=0;
			}
			break;
		}

		case eGCID_STOP_TIMER:
		{
			xTRACE("CGoEditorApp::OnCommand(eGCID_STOP_TIMER) received");
			unsigned int* buffer = (unsigned int*)param1;
			HandleUser hUser = (HandleUser)buffer[0];
			sGoUserInfo* info = (sGoUserInfo*)hUser;

			if (m_goban->IsCurrentBlack()==m_goban->ImBlack(info->goid))
			{
				StopTimer(info->goid);
				xTRACE("CGoEditorApp::OnCommand(eGCID_STOP_TIMER) StopTimer....1");
				yifree(buffer);
				return True;
			}

			unsigned int nUsedTime = buffer[2];
			m_goban->StopTimer(info->goid, nUsedTime);
				xTRACE("CGoEditorApp::OnCommand(eGCID_STOP_TIMER) StopTimer....2");
			yifree(buffer);
			break;
		}
		/*case eGCID_ADD_FRIEND:
		{
			// この場合はNotify.
			if (from==m_main)
			{
				//sGeneralParam* param = (sGeneralParam*)param1;
				//sMsg msg;
				//
				//m_network->m_msg->addref();
				break;
			}

			sGeneralParam param;
			param.param[0]=m_network->m_myinfo.id;
			param.param[1]=False;
			param.param[2]=CGoEditorApp::IdToInteger((const char*)param1);
			strncpy((char*)&param.param[3], (const char*)param1, 16);

			if (m_network->SendToServer(eUSPID_REQ_ADD_FRIEND, &param, GP_SIZE(3)+16, False)==False)
			{
				// どうする？
				yifree((void*)param1);
				return False;
			}
			yifree((void*)param1);
			break;
		}*/
		//case eGCID_GET_FRIEND_LIST:
		case eGCID_GET_FRIEND_LIST:
		{
			break;
		}

		case eGCID_ADD_FRIEND:
		{
			// ここで友達リスト更新
			if (from==m_main)
			{
				sGeneralParamEx* param = (sGeneralParamEx*)param1;
				cglsToolContainer* con = m_goban->GetContainer();
				char* pszUserName = (char*)param->data;//param[2];
				wchar_t pwszUserName[16];
				yiToUnicode(pwszUserName, pszUserName, 0);
				
				//con->SetHiLight(True);

				
				char buffer[1024];
				sMsg* msg = (sMsg*)buffer;
				msg->type=0;
				msg->remoteId=0;
				msg->szUserName[0]=0;

				if (param->param==0)
				{
					wchar_t buffer[64];
					wsprintfW((wchar_t*)buffer, L"%sさんの友人登録をリクエストしました。", pwszUserName);
					MakeMessage(msg, 0, 0, pszUserName, buffer);
					con->AddMessage(msg, True);
				}
				else if (param->param==-1)
				{
					wchar_t buffer[64];
					wsprintfW(buffer, L"友人のID(%s)が存在しません。", pwszUserName);
					MakeMessage(msg, 0, 0, pszUserName, buffer);
					con->AddMessage(msg, True, L"友人登録失敗");
				}
				else if (param->param==1)
				{
					wchar_t buffer[64];
					wsprintfW(buffer, L"%sさんにはすでに友人登録を申し込んでいます。", pwszUserName);
					MakeMessage(msg, 0, 0, pszUserName, buffer);
					con->AddMessage(msg, True);
				}
				else
				{
					wchar_t buffer[64];
					msg->type=1;
					wsprintfW(buffer, L"システムの障害により%sさんへの友人リクエストが失敗しました。\r\n恐れ入りますが、もう一度やり直して下さい。", pwszUserName);
					MakeMessage(msg, 0, 0, pszUserName, buffer);
					con->AddMessage(msg, True, L"友人登録失敗");
				}

				yifree(param);
			}
			else
			{
				sGeneralParam param;
				param.param[0]=m_myId;
				param.param[1]=False;
				param.param[2]=CGoEditorApp::IdToInteger((const char*)param1);
				strncpy((char*)&param.param[3], (const char*)param1, 16);

				if (CUserManager::GetInstance()->GetMyFriendId((const char*)param1)!=0)
				{
					char xbuffer[1024];
					sMsg* msg = (sMsg*)xbuffer;
					wchar_t pwszUserName[16];
					yiToUnicode(pwszUserName, (const char*)param1, 0);

					msg->type=0;
					msg->remoteId=0;
					msg->szUserName[0]=0;

					wchar_t buffer[64];
					wsprintfW((wchar_t*)buffer, L"%sさんはすでに友人に登録されています。", pwszUserName);
					MakeMessage(msg, 0, 0, (const char*)param1, buffer);
					m_goban->GetContainer()->AddMessage(msg, True);
					yifree((void*)param1);
					break;
				}

				if (m_network->SendToServer(eUSPID_REQ_ADD_FRIEND, &param, GP_SIZE(3)+16, False)==False)
				{
					// どうする？
					yifree((void*)param1);
					return False;
				}
				yifree((void*)param1);
				break;
			}
			break;
		}
		//case eGCID_NOTIFY_MSG32:
		//{
		//	break;
		//}
		//case eGCID_GET_MSG32:
		//{
		//	con->AddMessage(msg, True);

		//	break;
		//}
		case eGCID_ADD_GOBAN:
		{
			
			if (from==m_goban||
					from==0)
			{
				char* pszFileName = (char*)param1;
				sGoHeaderEx hd;
				char szNewFileName[256];
				strcpy(szNewFileName, yiPathGetFileName(pszFileName));

				char path[256];
				wsprintf(path, "%s\\data\\other\\%s", m_szUserDir, szNewFileName);

				Bool dir;
				if (yiIsExist(path, &dir)==True)
				{
					// 既に存在するファイルは取り込まないか、ファイル名を変更するが、
					// とりあえずははじく。
					return False;
				}

				if (CopyFile(pszFileName, path, TRUE)==FALSE)
				{
					// 何か問題？
					return False;
				}

				if (m_goban->ReadHeader(path, &hd)==False)
				{
					// 何か問題？
					return False;
				}

				strcpy(pszFileName, path);
				
				if (SaveGoData(1, &hd.header, szNewFileName, hd.map, hd.n)==False)
				{
					return False;
				}
				return True;

			}
			else if (from==m_goban->GetMyView(1))
			{
				char path[256];
				Bool dir;
				cglsMyFriendView* view = (cglsMyFriendView*)m_goban->GetMyView(1);
				unsigned int userId = view->GetCurrentFriendId();

				if (userId==0)
					return False;

				wsprintf(path, "%s\\cached\\data", m_szUserDir);
				if (yiIsExist(path, &dir)==False)
				{
					yiMakeDir(path);
				}

				sSimpleGoHeader* header = (sSimpleGoHeader*)param2;
				Bool bIsBlack = ((header->result&0x80000000)!=0);
				unsigned int b_id = bIsBlack?userId:header->remoteId;
				unsigned int w_id = bIsBlack?header->remoteId:userId;
				char ngf[256];
				int size;
				CUserManager* mgr = CUserManager::GetInstance();
				wsprintf(ngf, "%s\\%08x%08x%08x%08x.ngfx", path, b_id, w_id, header->day, header->time);

				sSimpleGoData* data = (sSimpleGoData*)yiFileLoadEx(ngf, &size);

				if (data)
				{
					if (m_goban->AddForEditBySimpleGodata(data, size, mgr->GetUserName(b_id), mgr->GetUserName(w_id))!=0)
					{
						yiFileUnLoadEx(data);
						break;
					}
					yiFileUnLoadEx(data);
				}

				ugoid_t goid(b_id, w_id);
				goid.day=header->day;
				goid.time=header->time;

				data = GetGoData(goid, &size);

				if (data==0)
				{
					break;
				}

				yiFileCreateEx(ngf, 0, data, size, True);					
				if (data)
				{
					yiFileCreateEx(ngf, 0, data, size, True);
					if (m_goban->AddForEditBySimpleGodata(data, size, mgr->GetUserName(b_id), mgr->GetUserName(w_id))!=0)
					{
						break;
					}
				}


				return True;
			}
			else
			{
				const char* pszFileName = (const char*)param1;
				char path[256];

				if (param2==0)
				{
					wsprintf(path, "%s\\data\\other\\%s", m_szUserDir,pszFileName);
				}
				else
				{
					wsprintf(path, "%s\\data\\%s", m_szMyDir,pszFileName);
				}
				sGobanContentsDetail detail;
				cyiGoEngine::xDefaultDetail(&detail);
				detail.eFormat=eGFT_SGF;
				goid_t id = m_goban->AddForEditByFile(path);

				// file not found?
				if (id==0 &
						param2!=0)
				{
					sSimpleGoHeader* header = (sSimpleGoHeader*)param2;
					Bool bIsBlack = ((header->result&0x80000000)!=0);
					unsigned int b_id = bIsBlack?CUserManager::GetInstance()->GetMyId():header->remoteId;
					unsigned int w_id = bIsBlack?header->remoteId:CUserManager::GetInstance()->GetMyId();
					char ngf[256];
					int size;
					CUserManager* mgr = CUserManager::GetInstance();
					wsprintf(ngf, "%s\\data\\%08x%08x%08x%08x.ngfx", m_szMyDir, b_id, w_id, header->day, header->time);

					sSimpleGoData* data = (sSimpleGoData*)yiFileLoadEx(ngf, &size);

					if (data)
					{
						if (m_goban->AddForEditBySimpleGodata(data, size, mgr->GetUserName(b_id), mgr->GetUserName(w_id))!=0)
						{
							yiFileUnLoadEx(data);
							break;
						}
						yiFileUnLoadEx(data);
					}

					ugoid_t goid(b_id, w_id);
					goid.day=header->day;
					goid.time=header->time;

					data = GetGoData(goid, &size);

					if (data==0)
					{
						break;
					}

					//yiFileCreateEx(ngf, 0, data, size, True);					
					if (data)
					{
						yiFileCreateEx(ngf, 0, data, size, True);
						if (m_goban->AddForEditBySimpleGodata(data, size, mgr->GetUserName(b_id), mgr->GetUserName(w_id))!=0)
						{
							break;
						}
					}

					break;

				}
			}
			break;
		}
		case eGCID_CONNECT_ME:
		{
			sGeneralParam* param = (sGeneralParam*)param1;
			unsigned int remoteId = param->param[0];
			address_t addr = param->param[1];
			int		port = param->param[2];
			sypnUserInfo remote;
			
			sGeneralParam buffer={1, 0};

			int sock = yiudp(0, 0);
			yisendu(sock, &buffer, 32, addr, port);

			sypnUserInfo info;

			if (m_network->AcceptFrom(sock, &info)==True)
			{
				CUserManager::GetInstance()->AddUserHandle((Handle)m_network->IndexToUser(info.index));
			}

			break;
		}
		case eGCID_NOTIFY_MSG32:
		{
			sMsg32* msg = 0;
			int index=-1;

			//if (from==0)
			//{
			msg = (sMsg32*)param1;
			//}
			//else
			//{
			//	unsigned int* param = (unsigned int*)param1;
			//	msg = (sMsg32*)&param[2];
			//}
			index = CUserManager::GetInstance()->AddMsg32(msg);
			
			wchar_t caption[256];
			char buffer[1024];
			sMsg* xx = (sMsg*)buffer;
			Msg32ToMessage(msg, index, xx, caption);

			m_goban->GetContainer()->AddMessage(xx, True, caption);

			if (from==m_main)
			{
				yifree(msg);
			}
			
			break;
		}
		case eGCID_DEL_MSG32:
		{
			//m_network->SendToServer(eUSPID_REQ_DEL_MSG32 ,&param1, sizeof(unsigned int));
			CUserManager::GetInstance()->DeleteMsg32(param1);
			break;
		}
		case eGCID_MSG_EXEC:
		{
			sMsg* msg = (sMsg*)param1;

			if ((msg->type>>16)==eWMT_ADD_FRIEND)
			{
				int bIsOK=param2;
				sGeneralParam param= {m_network->m_myinfo.id, param2?True:-1, msg->remoteId, 0, };
				
				yiToMultiByteCode((char*)&param.param[3], msg->szUserName, 0);
				m_network->SendToServer(eUSPID_REQ_ADD_FRIEND, &param, GP_SIZE(7), False);
			}
			break;
		}
		case eGCID_ADD_FRIEND_RESULT:
		{
			if (from==m_main)
			{
				// この場合はOK
				//unsigned int* xx = (unsigned int*)param1;
				sAddFriendOk* info= (sAddFriendOk*)param1;

				CUserManager::GetInstance()->SetRegisterInfo(&info->reg);
				CUserManager::GetInstance()->SetMyFriends(info->list);

				CUserManager::GetInstance()->AddDictionary(info->reg.id, info->reg.szUserName, 0);

				sypnUserInfo ypnUser;
				if (CUserManager::GetInstance()->GetUserHandle(info->reg.id)==0)
				{
					if (m_network->Connect(info->reg.id, 0, 0, 0, &ypnUser)==0)
					{
						CUserManager::GetInstance()->AddUserHandle((Handle)m_network->IndexToUser(ypnUser.index));
					}
				}
				m_goban->UpdateMyView(1, 0, 0, 0xFF);
				//m_goban->SetMyFriendView(CUserManager::GetInstance());
				//xAddMyFriend(info);

				yifree((void*)param1);
	
				m_goban->UpdateMyView(1, info->reg.id, 0, 0xFF);
				break;
			}
			else
			{
				// この場合はNG
				// 何もしない？
				break;
			}
		}
		case eGCID_REQ_PICTURE:
		{
			unsigned int* x=(unsigned int*)param1;
			unsigned int seq;
			HandleUser hUser=(HandleUser)x[0];
			sGoUserInfo* s = (sGoUserInfo*)hUser;
			Bitmap* bmp = CUserManager::GetInstance()->GetPicture(0, seq);

			if (bmp==0 ||
					seq==0)
			{
				yifree(x);
				break;
			}

			if (x[2]==seq)
			{
				yifree(x);
				break;
			}
			
			int size;
			void* p = CUserManager::GetInstance()->GetPictureFile(s->id, &size);

			if (p!=0 &&
				size!=0)
			{
				char* pSend = (char*)yialloci(size+sizeof(unsigned int));
				memcpy(pSend, &seq, sizeof(unsigned int));
				memcpy(&pSend[sizeof(unsigned int)], p, size);

				m_network->SendToUserEx(hUser, eGNPMSG_RPY_PICTURE, pSend, size+sizeof(unsigned int));

				yifree(p);
				yifree(pSend);
			}
		
			yifree(x);

			break;
		}
		case eGCID_RPY_PICTURE:
		{
			unsigned int* x=(unsigned int*)param1;
			void* p = (void*)&x[3];
			unsigned int seq = x[2];
			HandleUser hUser=(HandleUser)x[0];
			sGoUserInfo* s = (sGoUserInfo*)hUser;
			int size=x[1];

			CUserManager::GetInstance()->SetPictureFile(s->id, p, size-sizeof(unsigned int), seq);
			yifree(x);

			m_goban->UpdateMyView(1, s->id, 0, eMFVU_PICTURE); 
			break;
		}
		case eGCID_REQ_GODATA:
		{
			break;
		}
		case eGCID_RPY_GODATA:
		{
			break;
		}
		case eGCID_REQ_DATA:
		{
			unsigned int seq;
			unsigned int* param = (unsigned int*)(param1);

			HandleUser hUser=(HandleUser)param[0];
			int size = param[1];
			sGeneralParam* x = (sGeneralParam*)&param[2];

			int id = x->param[0];
			address_t addr=x->param[1];
			int port = x->param[2];

			seq=x->param[5];

			// 同じルータ内。
			if (m_network->GetMyInfo()->network.address==addr)
			{
				address_t	local_addr = x->param[3];
				int			local_port = x->param[4];
				xTRACE("OnCommand(eGCID_REQ_DATA) external address same! local addr is %u.%u.%u.%u, port:%d", 
						(local_addr>>24)&0xFF, (local_addr>>16)&0xFF, (local_addr>>8)&0xFF, local_addr&0xFF, local_port);
				SendDataToUser(hUser, id, local_addr, local_port, seq);
			}
			else
			{
				SendDataToUser(hUser, id, addr, port, seq);
			}

			yifree(param);

			break;
		}
		case eGCID_RPY_DATA:
		{
			unsigned int* param = (unsigned int*)(param1);
			sGeneralParam* p = (sGeneralParam*)&param[2];
			
			xOnRequestDataFrom((HandleUser)param[0], p);
			yifree(param);
			break;
		}
		case eGCID_DATA_RECEIVED:
		{
			unsigned int* xxx = (unsigned int*)((param1<<16)|(param2));

			xTRACE("OnCommand(eGCID_DATA_RECEIVED) %x", xxx);
			HandleUser hUser=(HandleUser)xxx[0];
			int size = xxx[1];
			int id	 = xxx[2];
			void* p  = (void*)xxx[3];

			OnDataReceived(hUser, id, p, size, xxx[4]);
			yifree(p);
			yifree(xxx);
			break;
		}

		default:
			return False;
	}

	return True;
}

#endif