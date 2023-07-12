#include "convar.h"
#include "icvar.h"

void CConVars::Init()
{
	cl_interp = g_ConVars.FindVar("cl_interp");
	cl_sidespeed = g_ConVars.FindVar("cl_sidespeed");
	cl_interp_ratio = g_ConVars.FindVar("cl_interp_ratio");
	cl_updaterate = g_ConVars.FindVar("cl_updaterate");
	cl_cmdrate = g_ConVars.FindVar("cl_cmdrate");
	tf_crit_cap = g_ConVars.FindVar("tf_weapon_criticals_bucket_cap");
	tf_crit_bottom = g_ConVars.FindVar("tf_weapon_criticals_bucket_bottom");
	tf_crit_default = g_ConVars.FindVar("tf_weapon_criticals_bucket_default");
	sv_gravity = g_ConVars.FindVar("sv_gravity");
	cl_flipviewmodels = g_ConVars.FindVar("cl_flipviewmodels");
	sv_maxunlag = g_ConVars.FindVar("sv_maxunlag");
	tf_enable_glows_after_respawn = g_ConVars.FindVar("tf_enable_glows_after_respawn");
	glow_outline_effect_enable = g_ConVars.FindVar("glow_outline_effect_enable");
	r_drawspecificstaticprop = g_ConVars.FindVar("r_drawspecificstaticprop");
	sv_namechange_cooldown_seconds = g_ConVars.FindVar("sv_namechange_cooldown_seconds");
	afkTimer = g_ConVars.FindVar("mp_idlemaxtime");
	sv_maxusrcmdprocessticks = g_ConVars.FindVar("sv_maxusrcmdprocessticks");
	sv_maxusrcmdprocessticks_holdaim = g_ConVars.FindVar("sv_maxusrcmdprocessticks_holdaim");

	ConCommandBase* cmdBase = I::CVar->GetCommands();
	while (cmdBase != nullptr) {
		cmdBase->m_nFlags &= ~(FCVAR_HIDDEN | FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT | FCVAR_NOT_CONNECTED);

		if (ConVar* convar = reinterpret_cast<ConVar*>(cmdBase)) {
			if (convar->GetName() == "name") {
				convar->m_fnChangeCallback = 0;
				convar->InternalSetValue(0);
				convar->m_fnChangeCallback = 0;
			}
			cvarMap[FNV1A::HashConst(convar->GetName())] = convar;
		}
		cmdBase = cmdBase->m_pNext;
	}
}

ConVar* CConVars::FindVar(const char* cvarname) {
	if (!cvarMap[FNV1A::HashConst(cvarname)]) {
		cvarMap[FNV1A::HashConst(cvarname)] = I::CVar->FindVar(cvarname);
	}
	return cvarMap[FNV1A::HashConst(cvarname)];
}

ConCommand *CConVars::FindCommand( const char *commandname ) {
	if ( !commandMap[ FNV1A::HashConst( commandname ) ] ) {
		commandMap[ FNV1A::HashConst( commandname ) ] = I::CVar->FindCommand( commandname );
	}
	return commandMap[ FNV1A::HashConst( commandname ) ];
}