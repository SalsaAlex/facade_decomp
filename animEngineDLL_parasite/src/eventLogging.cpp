#include "facadecommon.h"

#define MAX_EVENTLOGS 6000

int g_eventLogCtr = 0;
char g_eventLogFilename[100];

struct logevent_t
{
	uint32_t eventtime;
	EEventLogType logtype;
	ESpriteID perpetrator;
	char buffer[170];
	bool speechinterrupted;
	ESpriteID victim;
	bool outputted;
};
DEFINE_HOOKEDARRAYVAR(logevent_t, g_eventLog, 0x10235AD8)
#define g_eventLog _g_eventLog()

static void GetArticleForSpriteType(ESpriteID type, char* output)
{
	switch (type)
	{
		case eSpriteID_martiniDisplay1:
		case eSpriteID_martiniDisplay2:
		case eSpriteID_martiniDisplay3:
		case eSpriteID_martiniDisplay4:
		case eSpriteID_bar_redwinebottle:
		case eSpriteID_bar_whitewinebottle:
		case eSpriteID_playerDrink:
		case eSpriteID_tripDrink:
		case eSpriteID_graceDrink:
		case eSpriteID_trinket1:
		case eSpriteID_trinket2:
		case eSpriteID_trinket3:
		case eSpriteID_trinket4:
		case eSpriteID_trinket5:
		case eSpriteID_trinket6:
		case eSpriteID_trinket7:
		case eSpriteID_trinket8:
			strcpy(output, "a");
			break;
		default:
			strcpy(output, "the");
			break;
	}
}

void AddEventInfoToLog(EEventLogType type, ESpriteID perpetrator, 
	const char* text, bool speechinterrupted, 
	ESpriteID victim, uint32_t tick)
{
	static bool s_bPrintedErrorMessage = false;
	if (g_eventLogCtr >= MAX_EVENTLOGS)
	{
		if(s_bPrintedErrorMessage)
		{
			dprintf("### ERROR: Event log is FULL!");
			s_bPrintedErrorMessage = true;
		}
		return;
	}
	else if( (type == eEventLogType_Speech && text[0] == '\0')
			|| (type == eEventLogType_Speech && !strcmp(text, " "))
			|| (type == eEventLogType_unknown00 && text[0] == '\0')
			|| (type == eEventLogType_Miscellaneous && text[0] == '\0')
			|| (type == eEventLogType_Speech && !strcmp(text, "q"))		)
	{
		return;
	}

	animengine_assert(type <= eEventLogType_Miscellaneous);
	animengine_assert(perpetrator <= eSpriteID_LAST_AGENT
		|| perpetrator == eSpriteID_phone
		|| perpetrator == eSpriteID_answeringMachine
		|| type != eEventLogType_unknown00
		|| type == eEventLogType_Miscellaneous);

	int logindex = g_eventLogCtr;
	if (speechinterrupted
		&& perpetrator <= eSpriteID_LAST_CHARACTER
		&& g_eventLogCtr > 0
		&& g_eventLog[g_eventLogCtr - 1].perpetrator == eSpriteID_player)
	{
		//why didnt they just do a memcpy here?..
		g_eventLog[logindex].eventtime			= g_eventLog[logindex-1].eventtime;
		g_eventLog[logindex].logtype			= g_eventLog[logindex-1].logtype;
		g_eventLog[logindex].perpetrator		= g_eventLog[logindex-1].perpetrator;
		strcpy(g_eventLog[logindex].buffer, g_eventLog[logindex -1].buffer);
		g_eventLog[logindex].speechinterrupted = g_eventLog[logindex -1].speechinterrupted;
		g_eventLog[logindex].victim			= g_eventLog[logindex -1].victim;
		logindex--;
	}

	g_eventLog[logindex].eventtime = tick ? tick : GetTickCount();
	g_eventLog[logindex].logtype = type;
	g_eventLog[logindex].perpetrator = perpetrator;
	if (text)
		strcpy(g_eventLog[logindex].buffer, text);
	else
		g_eventLog[logindex].buffer[0] = '\0';
	g_eventLog[logindex].speechinterrupted = speechinterrupted;
	g_eventLog[logindex].victim = victim;
	g_eventLog[logindex].outputted = false;

	g_eventLogCtr++;
}
void OutputEventLog(bool stageplay)
{
	static bool s_bOutputtedStageplay = false;
	if (stageplay && s_bOutputtedStageplay)
		return;

	s_bOutputtedStageplay = true;

	for (int i = 0; i < g_eventLogCtr; i++)
		g_eventLog[i].outputted = false;

	time_t wintime;
	tm localwintime;
	char timestring[52];
	time(&wintime);
	memcpy(&localwintime, localtime(&wintime), sizeof(tm));
	strcpy(timestring, asctime(&localwintime));
	for (int i = 0; i < strlen(timestring); i++)
		timestring[i] = timestring[i] == ':' ? ' ' : timestring[i];


	char filepath[100];
	strcpy(filepath, "..\\..\\..\\stageplays\\");

	if(stageplay)	strcat(filepath, "Facade Stageplay ");
	else			strcat(filepath, "Facade Event Log ");

	strcat(filepath, timestring);
	strcat(filepath, ".txt");
	strcpy(g_eventLogFilename, filepath);

	FILE* hfile = fopen(filepath, "wt");
	if (!hfile)
	{
		dprintf("Could not open file for writing: %s\n", filepath);
		return;
	}

	char datastream[100];
	if(stageplay)	sprintf(datastream, "\nFACADE STAGEPLAY\n%s\n", timestring);
	else			sprintf(datastream, "\nFACADE LOG\n%s\n", timestring);

	fwrite(datastream, 1, strlen(datastream), hfile);

	uint32_t latesttime = 0;
	int last_eventindex = -1;
	for (int i = 0; i < g_eventLogCtr; i++) //i is unused here
	{
		int logindex = 0;
		for (int j = 0; j < g_eventLogCtr; j++)
		{
			if (!g_eventLog[j].outputted
				&& g_eventLog[j].eventtime >= latesttime
				&& (g_eventLog[j].eventtime <= latesttime || !latesttime))
			{
				latesttime = g_eventLog[j].eventtime;
				if (j - 1 < 0 || g_eventLog[j].eventtime != g_eventLog[j - 1].eventtime || g_eventLog[j].outputted)
					logindex = j;
			}
		}

		animengine_assert(latesttime != 0);

		logevent_t* eventlog = &g_eventLog[logindex];

		eventlog->outputted = true;

		extern bool g_bPrintBetaInfo;

		if ((!g_bPrintBetaInfo && stageplay && eventlog->logtype == eEventLogType_Speech)
			|| (stageplay && eventlog->logtype == eEventLogType_Miscellaneous))
		{
			return;
		}

		sprintf(datastream, "\n");
		fwrite(datastream, 1, strlen(datastream), hfile);
		if (!stageplay)
		{
			if (last_eventindex != -1
				&& (g_eventLog[last_eventindex].logtype == eEventLogType_unknown00 || g_eventLog[last_eventindex].logtype == eEventLogType_Miscellaneous)
				&& eventlog->logtype != eEventLogType_unknown00
				&& eventlog->logtype != eEventLogType_Miscellaneous)
			{
				sprintf(datastream, "\n");
				fwrite(datastream, 1, strlen(datastream), hfile);
			}
			last_eventindex = logindex;
			if (eventlog->logtype != eEventLogType_unknown00 && eventlog->logtype != eEventLogType_Miscellaneous)
				sprintf(datastream, "%d\n", eventlog->eventtime);
			else
				sprintf(datastream, "%d ", eventlog->eventtime);
			fwrite(datastream, 1, strlen(datastream), hfile);
		}
		
		char perpetratorname[52], perpetratorpronoun[12];
		const char* playername = g_PlayerSprite->GetName();
		switch (eventlog->perpetrator)
		{
			case eSpriteID_trip:	strcpy(perpetratorname, "Trip");	strcpy(perpetratorpronoun, "his"); break;
			case eSpriteID_grace:	strcpy(perpetratorname, "Grace");	strcpy(perpetratorpronoun, "her"); break;
			case eSpriteID_player: 
				strcpy(perpetratorname, playername);
				strcpy(perpetratorpronoun, "her");
				if(g_playerGender == GENDER_MALE)
					strcpy(perpetratorpronoun, "his");
				else
					strcpy(perpetratorpronoun, "her");
				break;
			default:	strcpy(perpetratorname, "???");	strcpy(perpetratorpronoun, "their");
				break;
		}

		char action[52], article[12];
		Sprite* obj = g_spriteMgr->GetSpritePtrByID(eventlog->victim);
		switch (eventlog->logtype)
		{
			case eEventLogType_unknown00:
			case eEventLogType_Miscellaneous:	sprintf(datastream, "%s", eventlog->buffer);	break;
			case eEventLogType_Speech:
				switch (eventlog->perpetrator)
				{
					case eSpriteID_grace: sprintf(datastream, "GRACE\n"); break;
					case eSpriteID_trip: sprintf(datastream, "TRIP\n"); break;
					case eSpriteID_player: sprintf(datastream, "%s\n", _strupr((char*)playername)); break;
					case eSpriteID_phone: sprintf(datastream, "PHONE\n"); break;
					case eSpriteID_answeringMachine: sprintf(datastream, "ANSWERING MACHINE\n"); break;
					default: break;
				}
				fwrite(datastream, 1, strlen(datastream), hfile);
				if (eventlog->speechinterrupted)
					sprintf(datastream, "%s%s\n", eventlog->buffer, " -- (interrupted)");
				else
					sprintf(datastream, "%s%s\n", eventlog->buffer, "");
				break;
			case eEventLogType_EnterInteraction:
				switch (eventlog->victim)
				{
					case eSpriteID_couch: strcpy(action, "sits on"); break;
					case eSpriteID_frontDoor: strcpy(action, "opens"); break;
					default: strcpy(action, "picks up"); break;
				}
				GetArticleForSpriteType(eventlog->victim, article);
				sprintf(datastream, "(%s %s %s %s.)\n", perpetratorname, action, article, obj->GetName());
				break;
			case eEventLogType_StopInteraction:
				switch (eventlog->victim)
				{
					case eSpriteID_couch: strcpy(action, "gets up from"); break;
					case eSpriteID_frontDoor: strcpy(action, "closes"); break;
					default: strcpy(action, "puts down"); break;
				}
				GetArticleForSpriteType(eventlog->victim, article);
				sprintf(datastream, "(%s %s %s %s.)\n", perpetratorname, action, article, obj->GetName());
				break;
			case eEventLogType_DoInteraction:
				switch (eventlog->victim)
				{
					case eSpriteID_eightball: strcpy(action, "shakes"); break;
					case eSpriteID_frontDoor: strcpy(action, "knocks on"); break;
					default:
						if(g_spriteMgr->GetSpritePtrByID(eventlog->victim)->a1_unknown00010)
							strcpy(action, "points to");
						else
							strcpy(action, "uses");
						break;
				}

				if (eventlog->victim == eSpriteID_eightball && eventlog->buffer[0] != '\0')
				{
					sprintf(datastream, "(The advice ball reads, \"%s\")\n", eventlog->buffer);
				}
				else
				{
					GetArticleForSpriteType(eventlog->victim, article);
					if (g_spriteMgr->GetSpritePtrByID(eventlog->victim)->m_eObjectType == eObjectType_Drink) //if drink is not empty ?
					{
						strcpy(action, "sips");
						strcpy(article, perpetratorpronoun);
					}
					sprintf(datastream, "(%s %s %s %s.)\n", perpetratorname, action, article, obj->GetName());
				}
				break;

			case eEventLogType_Hug:			sprintf(datastream, "(%s hugs %s.)\n", perpetratorname, obj->GetName());			break;
			case eEventLogType_Comfort:		sprintf(datastream, "(%s comforts %s.)\n", perpetratorname, obj->GetName());		break;
			case eEventLogType_Kiss:		sprintf(datastream, "(%s kisses %s.)\n", perpetratorname, obj->GetName());			break;
			case eEventLogType_ThrowDrink:	sprintf(datastream, "(%s throws drink at %s.)\n", perpetratorname, obj->GetName());	break;
			case eEventLogType_PlayerExit:	sprintf(datastream, "(%s quits the experience.)\n", perpetratorname);				break;
		}
		fwrite(datastream, 1, strlen(datastream), hfile);
	}


	fclose(hfile);
	dprintf("### Outputted event log");
}