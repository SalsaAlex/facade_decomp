#include "facadecommon.h"
#include "classes/SoundSource.h"

void SoundSource::GetSoundFilename(int soundindex, char* outbuffer, bool listpath)
{
	if (listpath)
		strcpy(outbuffer, m_listpath);
	else
		outbuffer[0] = '\0';

	if (!listpath || (strcmp(m_listpath, "sounds/grace/") && strcmp(m_listpath, "sounds/trip/")))
	{
		strcat(outbuffer, this->soundentries[soundindex].name);
	}
	else if (soundindex < 500)
	{
		strcat(outbuffer, "01/");
	}
	else if (soundindex > 500 && soundindex < 1000)
	{
		strcat(outbuffer, "02/");
	}
	else if(soundindex > 1000 && soundindex < 1500)
	{
		strcat(outbuffer, "03/");
	}
	else if (soundindex > 1500 && soundindex < 2000)
	{
		strcat(outbuffer, "04/");
	}
	else if (soundindex > 2000 && soundindex < 2500)
	{
		strcat(outbuffer, "05/");
	}
	else if (soundindex > 2500 && soundindex < 3000)
	{
		strcat(outbuffer, "06/");
	}
	else if (soundindex > 3000 && soundindex < 3500)
	{
		strcat(outbuffer, "07/");
	}
	else if (soundindex > 3500 && soundindex < 4000)
	{
		strcat(outbuffer, "08/");
	}
	else if (soundindex > 4000 && soundindex < 4500)
	{
		strcat(outbuffer, "09/");
	}
	else if (soundindex > 4500 && soundindex < 5000)
	{
		strcat(outbuffer, "10/");
	}
	else if (soundindex > 5000 && soundindex < 5500)
	{
		strcat(outbuffer, "11/");
	}
	else if (soundindex > 5500 && soundindex < 6000)
	{
		strcat(outbuffer, "12/");
	}

	
}
void SoundSource::InitSoundSource(const char* filepath)
{
	strcpy(m_listname, filepath);
	FILE *hfile = fopen(m_listname, "rt");
	if(!hfile)
		return dprintf("Could not open file for reading: %s\n", m_listname);

	char listbuffer[512];

	m_iNumSounds = -1;

	while (fgets(listbuffer, sizeof(listbuffer), hfile))
	{
		if (m_iNumSounds >= MAX_SOUNDENTRIES)
		{
			dprintf("Too many sounds in %s", hfile); //did they instead mean to put filepath here ?
			animengine_assert(0);
			break;
		}
		listbuffer[255] = '\0';
		listbuffer[strlen(listbuffer) - 1] = '\0';
		if (m_iNumSounds >= 0)
			strcpy(soundentries[m_iNumSounds].name, listbuffer);
		else
			strcpy(m_listpath, listbuffer);

		m_iNumSounds++;
	}
	
	fclose(hfile);
}