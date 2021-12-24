// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

    float FontSizeMultiplier = 1.0f;

	HOOK Hook_oCGame_UpdatePlayerStatus AS(&oCGame::UpdatePlayerStatus, &oCGame::Hooked_UpdatePlayerStatus);
    HOOK Hook_oCGame_OpenLoadscreen AS(&oCGame::OpenLoadscreen, &oCGame::Hooked_OpenLoadscreen);
    //HOOK Hook_oCGame_ClearGameState AS(&oCGame::ClearGameState, &oCGame::Hooked_ClearGameState);

    HOOK Hook_CGameManager_InitScreen_Open AS(&CGameManager::InitScreen_Open, &CGameManager::Hooked_InitScreen_Open);

    HOOK Hook_zCFont_GetFontData PATCH(&zCFont::GetFontData, &zCFont::GetFontData_Hooked);
    HOOK Hook_zCView_PrintChars PATCH(&zCView::PrintChars, &zCView::HookPrint);

    HOOK Hook_oCNpcFocus_IsValid AS(&oCNpcFocus::IsValid, &oCNpcFocus::Hook_IsValid);
    HOOK Hook_oCNpcFocus_GetPriority AS(&oCNpcFocus::GetPriority, &oCNpcFocus::Hook_GetPriority);
    HOOK Hook_oCNpc_GetNearestValidVob AS(&oCNpc::GetNearestValidVob, &oCNpc::Hook_GetNearestValidVob);
    HOOK Hook_oCNpc_GetNearestVob AS(&oCNpc::GetNearestVob, &oCNpc::Hook_GetNearestVob);

    HOOK Hook_zCAIBaseSound_StartDefaultCollisionSound AS(&zCAIBaseSound::StartDefaultCollisionSound, &zCAIBaseSound::Hook_StartDefaultCollisionSound);

    HOOK Hook_oCAniCtrl_Human_CheckMeleeWeaponHitsLevel AS(&oCAniCtrl_Human::CheckMeleeWeaponHitsLevel, &oCAniCtrl_Human::Hook_CheckMeleeWeaponHitsLevel);

    int zCFont::GetFontData_Hooked(const unsigned char letter, int& letterWidth, zVEC2& uvStart, zVEC2& uvEnd) {
        int ok = THISCALL(Hook_zCFont_GetFontData)(letter, letterWidth, uvStart, uvEnd);
        letterWidth *= FontSizeMultiplier;
        return ok;
    }

    void zCView::HookPrint(int x, int y, const zSTRING& text) {
        if (!font)
            return THISCALL(Hook_zCView_PrintChars)(x, y, text);

        // Condition
        float oldHeight = font->height;
        font->height *= FontSizeMultiplier;
        THISCALL(Hook_zCView_PrintChars)(x, y, text);
        font->height = oldHeight;
        FontSizeMultiplier = 1.0f;
    }

    void CGameManager::Hooked_InitScreen_Open()
    {

        if (initScreen) InitScreen_Close();

        // Hintergrundgrafik ermitteln und einbauen
        zSTRING backPic = "";

        initScreen = zNEW(zCView(0, 0, 8192, 8192));

        zoptions->ChangeDir(DIR_TEX_DESKTOP);
        screen->InsertItem(initScreen);
        initScreen->InsertBack("STARTSCREEN.TGA");

        zrenderer->BeginFrame();
        screen->Render();
        zrenderer->EndFrame();
        tagRECT* one = new tagRECT;
        tagRECT* two = new tagRECT;
        zrenderer->Vid_Blit(FALSE, one, two);
    };

    void oCGame::Hooked_OpenLoadscreen(bool gameStart, zSTRING worldName)
    {

        //THISCALL(Hook_oCGame_OpenLoadscreen)(gameStart, worldName);
        if (load_screen) CloseLoadscreen();

        if (gameStart)
        {
            load_screen = zNEW(zCView(0, 0, 8192, 8192));
            screen->InsertItem(load_screen);
            load_screen->InsertBack("STARTSCREEN.TGA");

            progressBar = zNEW(zCViewProgressBar(2000, 5700, 8192 - 2000, 6900, VIEW_ITEM));
            progressBar->SetMinimumStep(3);

            progressBar->SetPos(zPixelX((VID_MAX_XDIM / 2) - (40 / 2)), zPixelY(VID_MAX_YDIM - 240));
            progressBar->SetSize(zPixelX(40), zPixelY(40));
        }
        else
        {
            zFILE_FILE filePath(worldName);

            load_screen = zNEW(zCView(zPixelX((VID_MAX_XDIM/2)-(40/2)), zPixelY(VID_MAX_YDIM-240), zPixelX((VID_MAX_XDIM / 2) - (40 / 2) + 40), zPixelY(VID_MAX_YDIM - 240 + 40)));
            screen->InsertItem(load_screen);
            load_screen->InsertBack("LOG_PAPER.TGA");

            progressBar = zNEW(zCViewProgressBar(2000, 5700, 8192 - 2000, 6900, VIEW_ITEM));
            progressBar->SetMinimumStep(3);

            progressBar->SetPos(2000, 2000);
            progressBar->SetSize(8191, 8191);
            //zSTRING loadingBackground = zoptions->GetDirString(DIR_TEX_DESKTOP) + "loading_" + filePath.GetFilename() + ".tga";
            //loadingBackground.Lower();
            //load_screen->InsertBack(loadingBackground);

        }

        if (progressBar)
        {
            load_screen->InsertItem(progressBar);
            progressBar->Init();
        }

        screen->DrawItems();
        tagRECT* one = new tagRECT;
        tagRECT* two = new tagRECT;
        zrenderer->Vid_Blit(FALSE,one,two);

        if (GetWorld()) GetWorld()->progressBar = progressBar;
    };

	void oCGame::Hooked_UpdatePlayerStatus() {
		//THISCALL(Hook_oCGame_UpdatePlayerStatus)();

        // Disable default status bar
        screen->RemoveItem(ogame->hpBar);
        screen->RemoveItem(ogame->manaBar);
        screen->RemoveItem(ogame->swimBar);
        screen->RemoveItem(ogame->focusBar);

        FocusRender();

        if (!GetShowPlayerStatus())
        {
            statusBar->Close();
        }
        else
        {
            statusBar->Open();
        }

	}

	void FocusInit()
	{
		focusBar = new oCViewStatusBar();
        screen->InsertItem(focusBar);
		focusBar->Init(screen->anx(0), screen->any(0), 1.0F);
		focusBar->SetSize(screen->anx(80), screen->any(19));
        focusBar->SetRange(0, 100.0f);
        focusBar->SetMaxRange(0, 100.0f);
        focusBar->SetValue(100.0f);
		focusBar->SetTextures(BAR_BACK, "BAR_tempmax.tga", "BAR_HEALTH.TGA", "");
        screen->RemoveItem(focusBar);

	}

    void MenuRender()
    {
        if (!gameMan->IsGameRunning())
        {
            gameMan->initScreen->Print(10, 10, "CEngine 0.1 - by DocNight");
        }
    }

    BOOL oCNpcFocus::Hook_IsValid(int type)
    {
        if (type == zVOB_TYPE_NSC)	return (n_prio >= 0); else
            if (type == zVOB_TYPE_ITEM)	return (i_prio >= 0); else
                if (type == zVOB_TYPE_MOB)	return (m_prio >= 0);
                   // if (type == zVOB_TYPE_NORMAL) return (m_prio >= 0); else
                        // if (type == zVOB_TYPE_LEVEL_COMPONENT)	return (m_prio >= 0);
                      return FALSE;
    }

    int  oCNpcFocus::Hook_GetPriority(int type)
    {
        if (type == zVOB_TYPE_NSC)	return n_prio;	else
            if (type == zVOB_TYPE_ITEM)	return i_prio;	else
                if (type == zVOB_TYPE_MOB)	return m_prio;
                    //if (type == zVOB_TYPE_NORMAL) return (m_prio); else
                       // if (type == zVOB_TYPE_LEVEL_COMPONENT)	return (m_prio);
        return (-1);
    }

    void oCNpc::Hook_GetNearestValidVob(zREAL max_dist)
        // Only Valid, if its the player
    {
        static const int PICKVOBTIME = 0;				// alle halbe Sekunde

        zPOINT2 a_vobpos;
        if (pickvobdelay > PICKVOBTIME) {
            pickvobdelay -= PICKVOBTIME;
            // delete old List
    /*
            if (vobList.GetNumInList()!=0)
                // Folgende Zeile ist unbedingt noetig, damit keine Mem-Leaks entstehen !!
                vobList.DeleteList();
    */
            ClearVobList();

            zVEC3 pos1, pos2;
            // Create BBox
            zTBBox3D bbox;
            pos1 = bbox.maxs = bbox.mins = GetPositionWorld();
            // Vorderer Punkt	
            bbox.maxs[0] += max_dist; bbox.maxs[1] += max_dist; bbox.maxs[2] += max_dist;
            // Hinterer Punkt
            bbox.mins[0] -= max_dist; bbox.mins[1] -= max_dist; bbox.mins[2] -= max_dist;
            ogame->GetGameWorld()->bspTree.bspRoot->CollectVobsInBBox3D(vobList, bbox3D);;
            // Jetzt das naechste Vob ermitteln
    //		zREAL lowazi = 181.0;
    //		zREAL azi,elev;
            zCVob* vob;

            SetEnemy(NULL);
            ClearFocusVob();

            max_dist *= max_dist;			// Zum Quadrat

            int i, max = vobList.GetNumInList();
            for (i = 0; i < max; i++) {
                vob = vobList[i];

                if (vob != this) {
                    int type = vob->GetVobType();
                    if ((type == zVOB_TYPE_ITEM) && (((oCItem*)vob)->HasFlag(ITM_FLAG_NFOCUS))) continue;
                    if ((type == zVOB_TYPE_NSC) && (((oCNpc*)vob)->IsDead())) type = zVOB_TYPE_MOB;
                    if (!oCNpcFocus::focus->IsValid(type)) continue;

                    if (zDYNAMIC_CAST<zCVob>(vob)) {
                        // Entfernung okay ?
                        if (GetDistanceToVob2(*vob) <= max_dist) {
                            // FreeLineOfSight ?
                            zREAL angle = GetAngle(vob);
                            oCNpc* npc = zDYNAMIC_CAST<oCNpc>(vob);
                            zBOOL inValidNpc = FALSE;

                            if (npc && (GetWeaponMode() >= NPC_WEAPON_DAG && GetWeaponMode() <= NPC_WEAPON_2HS) && (npc->IsUnconscious() || npc->IsDead()))
                                inValidNpc = TRUE;

                            if (!inValidNpc && FreeLineOfSight(vob) && angle <= oCNpcFocus::focus->GetAzi())
                            {

                                max_dist = GetDistanceToVob2(*vob);
                                SetFocusVob(vob);
                                // [Ulf] : (AtHome2)
                                if ((type == zVOB_TYPE_NSC) && (!((oCNpc*)vob)->IsDead())) SetEnemy((oCNpc*)vob);
                            }
                        }
                    }
                }
            }

            vobList.Remove(this);

            for (i = vobList.GetNumInList() - 1; i >= 0; i--)
                zADDREF(vobList[i]);

        }
        else
            pickvobdelay += ztimer->frameTimeFloat;
    }

    void oCNpc::Hook_GetNearestVob(zREAL max_dist)
        // Only Valid, if its the player
    {
        static const int PICKVOBTIME = 0;				// alle halbe Sekunde

        zPOINT2 a_vobpos;
        if (pickvobdelay > PICKVOBTIME) {
            pickvobdelay -= PICKVOBTIME;
            // delete old List
    /*
            if (vobList.GetNumInList()!=0)
                // Folgende Zeile ist unbedingt noetig, damit keine Mem-Leaks entstehen !!
                vobList.DeleteList();
    */
            ClearVobList();

            zVEC3 pos1, pos2;
            // Create BBox
            zTBBox3D bbox;
            pos1 = bbox.maxs = bbox.mins = GetPositionWorld();
            // Vorderer Punkt	
            bbox.maxs[0] += max_dist; bbox.maxs[1] += max_dist; bbox.maxs[2] += max_dist;
            // Hinterer Punkt
            bbox.mins[0] -= max_dist; bbox.mins[1] -= max_dist; bbox.mins[2] -= max_dist;
            ogame->GetGameWorld()->bspTree.bspRoot->CollectVobsInBBox3D(vobList, bbox3D);;
            // Jetzt das naechste Vob ermitteln
    //		zREAL lowazi = 181.0;
    //		zREAL azi,elev;
            zCVob* vob;

            SetEnemy(NULL);
            ClearFocusVob();

            max_dist *= max_dist;			// Zum Quadrat

            int i, max = vobList.GetNumInList();
            for (i = 0; i < max; i++) {
                vob = vobList[i];

                if (vob != this) {
                    int type = vob->GetVobType();
                    if ((type == zVOB_TYPE_ITEM) && (((oCItem*)vob)->HasFlag(ITM_FLAG_NFOCUS))) continue;

                    if (zDYNAMIC_CAST<zCVob>(vob)) {
                        // Entfernung okay ?
                        if (GetDistanceToVob2(*vob) <= max_dist) {
                            // FreeLineOfSight ?
                            zREAL angle = GetAngle(vob);
                            if (FreeLineOfSight(vob) && angle <= oCNpcFocus::focus->GetAzi()) {
                                max_dist = GetDistanceToVob2(*vob);
                                SetFocusVob(vob);
                                // [Ulf] : (AtHome2)
                                if ((type == zVOB_TYPE_NSC) && (!((oCNpc*)vob)->IsDead())) SetEnemy((oCNpc*)vob);
                            }
                        }
                    }
                }
            }

            vobList.Remove(this);

            for (i = vobList.GetNumInList() - 1; i >= 0; i--)
                zADDREF(vobList[i]);

        }
        else
            pickvobdelay += ztimer->frameTimeFloat;
    }

    void zCAIBaseSound::Hook_StartDefaultCollisionSound(zCVob* vob, const zCCollisionReport& collisionReport)
    {
        int thisMat = 99;
        if (collisionReport.m_poCollObject1->GetCollObjClass() == zCCollObjectLevelPolys::S_GetCollObjClass())
        {
            zCCollObjectLevelPolys* collObjLevel = static_cast<zCCollObjectLevelPolys*> (collisionReport.m_poCollObject1);
           
            int levelMat = int(collObjLevel->m_aoPolyList[0]->material->matGroup);
            zsndMan->StartHitSound(vob, zCSoundManager::zSND_MAN_MEDIUM_OBJECT, zCSoundManager::zSND_MAN_MEDIUM_LEVEL, thisMat, levelMat, &collisionReport);
        }
        else
        {
            // damit nicht beide Vobs den Coll-Sound ausloesen, macht bei einer Coll. dies nur der mit der kleineren Adresse!
            // AUSNAHME: der andere Vob hat keine AI und kann deshalb auch keinen Sound triggern !
            zCVob* otherVob = static_cast<zCVob*>(collisionReport.m_poCollObject1->m_pvClientData);
            if ((vob < otherVob) || (!otherVob->callback_ai))
            {
                int otherVobMat = 99;
                zsndMan->StartHitSound(vob, zCSoundManager::zSND_MAN_MEDIUM_ITEM, zCSoundManager::zSND_MAN_MEDIUM_ITEM, thisMat, otherVobMat, &collisionReport);
            };
        };
    };

    zBOOL oCAniCtrl_Human::Hook_CheckMeleeWeaponHitsLevel(oCItem* a_pWeapon)
    {
        // egal in welchem weapon mode kann hier getestet werden, ob die attack coll limbs hier den
        // statischen level berühren
        // vorsicht: innerhalb einer combo darf nur einmal ein collisionseffekt gestartet werden
        if (!canDoCollisionFX)														return FALSE;
        if (IsInPostHit() || IsInCombo())											return FALSE;
        if (npc->GetHomeWorld() && npc->GetHomeWorld()->masterFrameCtr & 2)	return FALSE;

        zCList<zCVob::zTModelLimbColl> resCollList;
        zCVob::zTModelLimbColl* limbinfo;

        for (int i = 0; i < ANI_HITLIMB_MAX; i++)
        {
            if (hitlimb[i])
            {
                zVEC3		approxCollPos;
                zCMaterial* foundCollMaterial;

                npc->CheckModelLimbCollisionLevel(hitlimb[i], TRUE, resCollList);

                if (resCollList.GetNumInList() != 0)
                {
                    limbinfo = resCollList.GetNextInList()->GetData();
                    approxCollPos = limbinfo->approxCollisionPos;
                    foundCollMaterial = limbinfo->hitMaterial;

                    // Folgende Zeile ist unbedingt noetig, damit keine Mem-Leaks entstehen !!
                    resCollList.DeleteListDatas();

                    // coll effekte starten (pfx/sound)
                    zCSoundManager::zTSndManMedium	med1, med2;
                    oTSndMaterial					mat1, mat2;

                    npc->GetSoundMaterial_MA(med1, mat1, "FIST");

                    med2 = zCSoundManager::zSND_MAN_MEDIUM_LEVEL;

                    if (!foundCollMaterial) mat2 = SND_MAT_METAL;
                    else
                    {
                        switch (foundCollMaterial->matGroup)
                        {
                        case zMAT_GROUP_METAL: mat2 = SND_MAT_METAL; break;
                        case zMAT_GROUP_STONE: mat2 = SND_MAT_STONE; break;
                        case zMAT_GROUP_WOOD:  mat2 = SND_MAT_WOOD;  break;
                        case zMAT_GROUP_SNOW:  mat2 = SND_MAT_LEATHER; break;
                        case zMAT_GROUP_EARTH: mat2 = SND_MAT_CLAY;  break;
                        case zMAT_GROUP_WATER: mat2 = SND_MAT_GLAS;  break;
                        default:			   mat2 = SND_MAT_STONE; break;
                        }
                    }

                    //
                    //	CREATE PARTICLE FX
                    //
                    zCModelNodeInst* rightHand = GetModel()->SearchNode("ZS_RIGHTHAND");
                    if (rightHand)
                    {
                        zMATRIX4 trafoHandToWorld = npc->trafoObjToWorld * GetModel()->GetTrafoNodeToModel(rightHand);
                        zPOINT3	 slotpos = trafoHandToWorld.GetTranslation();
                        zBOOL inMove = FALSE;
                        if (npc->isInMovementMode != zCVob::zTMovementMode::zVOB_MOVE_MODE_NOTINBLOCK)
                        {
                            zBOOL inMove = TRUE;
                        }

                        //
                        //	CODECHANGE [ROMAN]
                        //
                        //if ( npc	->GetWeaponMode() != NPC_WEAPON_FIST ) 
                        {
                            //
                            //	COMPOSE MATERIAL NAME FROM WEAPON MATERIALS
                            //
                            zSTRING strMaterial;
                            zSTRING strMaterialSrc = SndMaterialName[mat1];
                            zSTRING strMaterialDst = SndMaterialName[mat2];

                            // vorher: altes system: mat der waffe mit berücksichtigt
                            /*strMaterial = strMaterialSrc;

                            if		( strMaterialSrc != strMaterialDst )
                                if	( strMaterialSrc  < strMaterialDst )	strMaterial = strMaterialSrc + "_" + strMaterialDst;
                                else										strMaterial = strMaterialDst + "_" + strMaterialSrc;*/

                                // nachher: nur das level material wird berücksichtigt, und aus CPFX_IAI_ genommen
                            strMaterial = strMaterialDst;

                            //
                            //	COMPOSE PFX NAME FROM MATERIAL NAMES
                            //
                            zSTRING strNameFX;

                            strNameFX = "CPFX_IAI_";
                            strNameFX += strMaterial;

                            //
                            //	START PROPER PFX
                            //
                            if (inMove) npc->EndMovement(FALSE);
                            this->DoSparks(slotpos, strNameFX);
                            if (inMove) npc->BeginMovement();
                        }
                        //
                    }

                    // Sound	
                    zsndMan->StartHitSound(npc, med1, med2, mat1, mat2, 0);
                    canDoCollisionFX = FALSE;
                    RemoveVobHit(limbinfo->hitVob);
                    return TRUE;
                }
            }
        }

        return FALSE;
    }

    void RemoveVobHit(zCVob* vob)
    {
        //oCWorld* world = ogame->GetGameWorld();
        //world->RemoveVobSubtree(vob);
        //world->RemoveVob(vob);
    }

	void FocusRender()
	{
        screen->RemoveItem(focusBar);

        if (!ogame->GetShowPlayerStatus()) return;

        if (player->IsDead())
        {
            screen->Print(3000, 4000, "Ïîïóùåí è îáîñàí...");
        }

        zBOOL show_FocusItm = FALSE;
        zBOOL show_FocusMob = FALSE;
        zBOOL show_FocusNpc = FALSE;
        zBOOL show_FocusBar = FALSE;

        zBOOL bShowFocus = FALSE;

        zSTRING text;
        zPOINT3 focusNamePosWS;
        zINT hp;
        zINT hpMax;
        zCVob* focus = oCNpc::player->GetFocusVob();
        //zCVob* focus = oCNpc::player;
        oCNpc* other = NULL;

        if (other = zDYNAMIC_CAST<oCNpc>(focus))
        {
            int mem = 0;
            text = other->GetName(mem);
            hp = other->GetAttribute(NPC_ATR_HITPOINTS);
            hpMax = other->GetAttribute(NPC_ATR_HITPOINTSMAX);
            bShowFocus = TRUE;
            show_FocusNpc = TRUE;
            show_FocusBar = TRUE;
        }
        else if (zDYNAMIC_CAST<oCMOB>(focus))
        {
            text = static_cast<oCMOB*>(focus)->GetName();
            bShowFocus = TRUE;
            show_FocusMob = TRUE;
        }
        else if (dynamic_cast<oCItem*>(focus))
        {
            text = static_cast<oCItem*>(focus)->GetName(TRUE);
            bShowFocus = TRUE;
            show_FocusItm = TRUE;
        }
        else if (zDYNAMIC_CAST<zCVob>(focus))
        {
            text = focus->visual->GetVisualName();
            bShowFocus = TRUE;
            show_FocusMob = TRUE;
        }

        if (!bShowFocus) return;

        if (focus && focus->GetVisual())
        {
            const zCClassDef* classDef0 = focus->GetVisual()->_GetClassDef();
            const zCClassDef* classDef1 = zCModel::classDef;

            if ((focus->GetVisual()) && (classDef0 == classDef1))
            {
                zCModel* model = static_cast<zCModel*>(focus->GetVisual());
                const zTBBox3D& modelBoxLocalFixed = model->bbox3DLocalFixed;
                focusNamePosWS = focus->GetPositionWorld();
                focusNamePosWS.n[VY] += (modelBoxLocalFixed.maxs[VY]) * 1.6F;

                const zREAL bboxTopWS = (focus->bbox3D.maxs[VY]) + zREAL(100.0F);
                if (focusNamePosWS.n[VY] > bboxTopWS)
                    focusNamePosWS.n[VY] = bboxTopWS;
            }
            else
            {
                focusNamePosWS = focus->bbox3D.GetCenter();
                focusNamePosWS.n[VY] += (focus->bbox3D.maxs.n[VY] - focus->bbox3D.mins.n[VY]) * 0.82F;
            };

            zCCamera::activeCam->Activate();
            zPOINT3	  ProjPoint1 = zCCamera::activeCam->camMatrix * focusNamePosWS;
            if (ProjPoint1[VZ] > 0.0f)
            {

                zPOINT2	  ProjPoint2;
                zCCamera::activeCam->Project(&ProjPoint1, ProjPoint2[VX], ProjPoint2[VY]);
                ProjPoint2[VX] = ProjPoint2[VX] - screen->nax(screen->FontSize(text + "\n") / 2);

                // CLAMP
                int nX = screen->anx(ProjPoint2[VX]);
                int nY = screen->any(ProjPoint2[VY]);

                if (nY < screen->FontY()) nY = screen->FontY();
                if (nY > 8192 - screen->FontY()) nY = 8192 - screen->FontY();

                // [Ulf] Clampen auch in X-Richtung
                zClamp(nX, 0, 8191 - screen->FontSize(text));

                // PRINT
                int aX = zVirtualX(nX);
                int aY = zVirtualY(nY);
                
                screen->Print(nX, nY, text + "\n");

                if (!show_FocusBar) return;

                focusBar->SetRange(0, hpMax);
                focusBar->SetMaxRange(0, hpMax);
                focusBar->SetValue(hp);

                int timePosX = zPixelX(aX);
                int timePosY = zPixelY(aY + screen->GetFont()->GetFontY());
                int timeSizeX = screen->anx(80);
                int timeSizeY = screen->any(18);

                if (screen->GetFont()->GetFontX(text) > 80)
                {
                    timeSizeX = screen->anx(screen->GetFont()->GetFontX(text));
                    timeSizeY = screen->any(18);
                };

                if ((timePosX + timeSizeX) > 8191)
                {
                    timePosX = 8191 - timeSizeX;
                }
                if ((timePosY + timeSizeY) > 8191)
                {
                    timePosY = 8191 - timeSizeY;
                }

                focusBar->SetPos(timePosX, timePosY);
                focusBar->SetSize(timeSizeX, timeSizeY);

                screen->InsertItem(focusBar);
                screen->RemoveItem(focusBar);

                screen->InsertItem(focusBar);
            };
        }
	}

}