/*
 *   IRC - Internet Relay Chat, src/modules/out.c
 *   (C) 2004 The UnrealIRCd Team
 *
 *   See file AUTHORS in IRC package for additional names of
 *   the programmers.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "unrealircd.h"

CMD_FUNC(m_admin);

#define MSG_ADMIN 	"ADMIN"	

ModuleHeader MOD_HEADER(admin)
  = {
	"admin",
	"5.0",
	"command /admin", 
	"UnrealIRCd Team",
	"unrealircd-5",
    };

MOD_INIT(admin)
{
	CommandAdd(modinfo->handle, MSG_ADMIN, m_admin, MAXPARA, M_UNREGISTERED|M_USER|M_SHUN|M_VIRUS);
	MARK_AS_OFFICIAL_MODULE(modinfo);
	return MOD_SUCCESS;
}

MOD_LOAD(admin)
{
	return MOD_SUCCESS;
}

MOD_UNLOAD(admin)
{
	return MOD_SUCCESS;
}

/*
** m_admin
**	parv[1] = servername
*/
CMD_FUNC(m_admin)
{
	ConfigItem_admin *admin;
	/* Users may want to get the address in case k-lined, etc. -- Barubary

	   * Only allow remote ADMINs if registered -- Barubary */
	if (IsPerson(sptr) || IsServer(cptr))
		if (hunt_server(cptr, sptr, recv_mtags, ":%s ADMIN :%s", 1, parc, parv) != HUNTED_ISME)
			return 0;

	if (!conf_admin_tail)
	{
		sendnumeric(sptr, ERR_NOADMININFO, me.name);
		return 0;
	}

	sendnumeric(sptr, RPL_ADMINME, me.name);

	/* cycle through the list backwards */
	for (admin = conf_admin_tail; admin; admin = admin->prev)
	{
		if (!admin->next)
			sendnumeric(sptr, RPL_ADMINLOC1,
			    me.name, sptr->name, admin->line);
		else if (!admin->next->next)
			sendnumeric(sptr, RPL_ADMINLOC2,
			    me.name, sptr->name, admin->line);
		else
			sendnumeric(sptr, RPL_ADMINEMAIL, admin->line);
	}
	return 0;
}
