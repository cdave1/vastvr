#include "Core/Application/Client.h"

#include "hdk.h"
#include "Game.h"

static hdGame *m_game = NULL;


using namespace hd::application;

void Client::Init()
{
    m_game = new hdGame(this);
    
    hdInitImmediateModeGL();
}


void Client::WillShutdown()
{
    
}


void Client::Shutdown()
{
    
}


void Client::Pause()
{
    
}


void Client::Resume()
{
    
}


void Client::Update()
{
    // Pulls list of entities of event queue and sends events to various
    // sub systems.
    UpdateEventQueue();
    
    m_game->Update();
    
    UpdateTimer();
}


void Client::Render()
{
    glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    
    glPointSize(1.0f);
    
    m_game->Draw();
    
    GetEventQueue()->DebugRender();
    
}
