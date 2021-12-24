// This file added in headers queue
// File: "Sources.h"
#include "resource.h"
#include <iostream>
#include "Plugin.h"

namespace GOTHIC_ENGINE {

  void Game_Entry() {
  }
  
  void Game_Init() {
      statusBar = new CStatusBar(VID_MAX_XDIM/2-200, VID_MAX_YDIM-80);

      cursor = new Cursor();
      cursor->SetTexture("LO.TGA");
      //ogame->load_screen->InsertBack("LOADING_ADDONWORLD-C.TEX");
      //gameMan->initScreen->InsertBack("LOADING_ADDONWORLD.TGA");

      FocusInit();

      //worldVob = new CWorld();
      //worldVob->InsertVob("NW_NATURE_BIGTREE_356P.3DS");
  }

  void Game_Exit() {
  }

  void Game_PreLoop() {
  }

  void Game_Loop() {
      statusBar->GameRender();
      cursor->Render();

#ifdef _DEBUG
      oCWorld* world = ogame->GetGameWorld();
      world->TraceRayNearestHit(player->GetPositionWorld(), player->GetAtVectorWorld() * 200.0f, (zCArray<zCVob*>*)0, FALSE);
      
      zCVob* foundVob = world->traceRayReport.foundVob;

      if (foundVob)
          foundSctring = foundVob->visual->GetVisualName();
      else
          foundSctring = "NULL";

      screen->Print(10, 10, foundSctring);
     
      if (zinput->KeyToggled(KEY_I))
      {
          zCQuat rot = zCQuat(0,0,0,0);
          zVEC3 vecRot = player->GetAtVectorWorld();
          rot.EulerToQuat(vecRot);

          rot.q[VY] = 0; // конвертнуть радианы Z и X в угол для Y
          rot.q[VZ] = 0;
          rot.q[VX] = 0;

          //oCVisualFX* blendFX = oCVisualFX::CreateAndPlay("BLACK_SCREEN", oCNpc::player, FALSE, FALSE, FALSE, FALSE, FALSE);
          //zRELEASE(blendFX);
          

          //worldVob->CreateVob(player->GetVisual(), player->GetPositionWorld() + player->GetAtVectorWorld() * 1000.0f, rot, True);
      }
#endif
     
  }

  void Game_PostLoop() {

  }

  void Game_MenuLoop() {
      MenuRender();
  }

  // Information about current saving or loading world
  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

  void Game_SaveBegin() {
  }

  void Game_SaveEnd() {
  }

  void LoadBegin() {
      statusBar->Close();
      //cursor->SetVisible(false);
  }

  void LoadEnd() {
      statusBar->Open();
      //cursor->SetVisible(true);
  }

  void Game_LoadBegin_NewGame() {
    LoadBegin();
  }

  void Game_LoadEnd_NewGame() {
    LoadEnd();
  }

  void Game_LoadBegin_SaveGame() {
    LoadBegin();
  }

  void Game_LoadEnd_SaveGame() {
    LoadEnd();
  }

  void Game_LoadBegin_ChangeLevel() {
    LoadBegin();
  }

  void Game_LoadEnd_ChangeLevel() {
    LoadEnd();
  }

  void Game_LoadBegin_Trigger() {
  }
  
  void Game_LoadEnd_Trigger() {
  }
  
  void Game_Pause() {
  }
  
  void Game_Unpause() {
  }
  
  void Game_DefineExternals() {
  }

  void Game_ApplyOptions() {
  }

  /*
  Functions call order on Game initialization:
    - Game_Entry           * Gothic entry point
    - Game_DefineExternals * Define external script functions
    - Game_Init            * After DAT files init
  
  Functions call order on Change level:
    - Game_LoadBegin_Trigger     * Entry in trigger
    - Game_LoadEnd_Trigger       *
    - Game_Loop                  * Frame call window
    - Game_LoadBegin_ChangeLevel * Load begin
    - Game_SaveBegin             * Save previous level information
    - Game_SaveEnd               *
    - Game_LoadEnd_ChangeLevel   *
  
  Functions call order on Save game:
    - Game_Pause     * Open menu
    - Game_Unpause   * Click on save
    - Game_Loop      * Frame call window
    - Game_SaveBegin * Save begin
    - Game_SaveEnd   *
  
  Functions call order on Load game:
    - Game_Pause              * Open menu
    - Game_Unpause            * Click on load
    - Game_LoadBegin_SaveGame * Load begin
    - Game_LoadEnd_SaveGame   *
  */

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_PreLoop,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_PostLoop,
    Enabled( AppDefault ) Game_MenuLoop,
    Enabled( AppDefault ) Game_SaveBegin,
    Enabled( AppDefault ) Game_SaveEnd,
    Enabled( AppDefault ) Game_LoadBegin_NewGame,
    Enabled( AppDefault ) Game_LoadEnd_NewGame,
    Enabled( AppDefault ) Game_LoadBegin_SaveGame,
    Enabled( AppDefault ) Game_LoadEnd_SaveGame,
    Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
    Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
    Enabled( AppDefault ) Game_LoadBegin_Trigger,
    Enabled( AppDefault ) Game_LoadEnd_Trigger,
    Enabled( AppDefault ) Game_Pause,
    Enabled( AppDefault ) Game_Unpause,
    Enabled( AppDefault ) Game_DefineExternals,
    Enabled( AppDefault ) Game_ApplyOptions
  );
}