/*
 * Copyright (C) 2012 Tobias Brunner
 * Copyright (C) 2006 Martin Willi
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include <stdlib.h>
#include <errno.h>

#include "eap.h"

#include <debug.h>

ENUM(eap_code_names, EAP_REQUEST, EAP_FAILURE,
	"EAP_REQUEST",
	"EAP_RESPONSE",
	"EAP_SUCCESS",
	"EAP_FAILURE",
);

ENUM(eap_code_short_names, EAP_REQUEST, EAP_FAILURE,
	"REQ",
	"RES",
	"SUCC",
	"FAIL",
);

ENUM_BEGIN(eap_type_names, EAP_IDENTITY, EAP_GTC,
	"EAP_IDENTITY",
	"EAP_NOTIFICATION",
	"EAP_NAK",
	"EAP_MD5",
	"EAP_OTP",
	"EAP_GTC");
ENUM_NEXT(eap_type_names, EAP_TLS, EAP_TLS, EAP_GTC,
	"EAP_TLS");
ENUM_NEXT(eap_type_names, EAP_SIM, EAP_SIM, EAP_TLS,
	"EAP_SIM");
ENUM_NEXT(eap_type_names, EAP_TTLS, EAP_TTLS, EAP_SIM,
	"EAP_TTLS");
ENUM_NEXT(eap_type_names, EAP_AKA, EAP_AKA, EAP_TTLS,
	"EAP_AKA");
ENUM_NEXT(eap_type_names, EAP_PEAP, EAP_MSCHAPV2, EAP_AKA,
	"EAP_PEAP",
	"EAP_MSCHAPV2");
ENUM_NEXT(eap_type_names, EAP_MSTLV, EAP_MSTLV, EAP_MSCHAPV2,
	"EAP_MSTLV");
ENUM_NEXT(eap_type_names, EAP_TNC, EAP_TNC, EAP_MSTLV,
	"EAP_TNC");
ENUM_NEXT(eap_type_names, EAP_EXPANDED, EAP_DYNAMIC, EAP_TNC,
	"EAP_EXPANDED",
	"EAP_EXPERIMENTAL",
	"EAP_RADIUS",
	"EAP_DYNAMIC");
ENUM_END(eap_type_names, EAP_DYNAMIC);

ENUM_BEGIN(eap_type_short_names, EAP_IDENTITY, EAP_GTC,
	"ID",
	"NTF",
	"NAK",
	"MD5",
	"OTP",
	"GTC");
ENUM_NEXT(eap_type_short_names, EAP_TLS, EAP_TLS, EAP_GTC,
	"TLS");
ENUM_NEXT(eap_type_short_names, EAP_SIM, EAP_SIM, EAP_TLS,
	"SIM");
ENUM_NEXT(eap_type_short_names, EAP_TTLS, EAP_TTLS, EAP_SIM,
	"TTLS");
ENUM_NEXT(eap_type_short_names, EAP_AKA, EAP_AKA, EAP_TTLS,
	"AKA");
ENUM_NEXT(eap_type_short_names, EAP_PEAP, EAP_MSCHAPV2, EAP_AKA,
	"PEAP",
	"MSCHAPV2");
ENUM_NEXT(eap_type_short_names, EAP_MSTLV, EAP_MSTLV, EAP_MSCHAPV2,
	"MSTLV");
ENUM_NEXT(eap_type_short_names, EAP_TNC, EAP_TNC, EAP_MSTLV,
	"TNC");
ENUM_NEXT(eap_type_short_names, EAP_EXPANDED, EAP_DYNAMIC, EAP_TNC,
	"EXP",
	"XP",
	"RAD",
	"DYN");
ENUM_END(eap_type_short_names, EAP_DYNAMIC);

ENUM(eap_vendor_names_unknown, 0, 0,
	"(0)",
);

ENUM(eap_vendor_names_ms, EAP_MS_SOH, EAP_MS_CAPABILITES,
	"MS-SOH",
	"MS-CAP",
);

/*
 * See header
 */
enum_name_t* eap_type_get_names(pen_t vendor)
{
	switch (vendor)
	{
		case PEN_IETF:
			return eap_type_short_names;
		case PEN_MICROSOFT:
			return eap_vendor_names_ms;
		default:
			return eap_vendor_names_unknown;
	}
}

/*
 * See header
 */
eap_type_t eap_type_from_string(char *name)
{
	int i;
	static struct {
		char *name;
		eap_type_t type;
	} types[] = {
		{"identity",	EAP_IDENTITY},
		{"md5",			EAP_MD5},
		{"otp",			EAP_OTP},
		{"gtc",			EAP_GTC},
		{"tls",			EAP_TLS},
		{"ttls",		EAP_TTLS},
		{"sim",			EAP_SIM},
		{"aka",			EAP_AKA},
		{"peap",		EAP_PEAP},
		{"mschapv2",	EAP_MSCHAPV2},
		{"tnc",			EAP_TNC},
		{"dynamic",		EAP_DYNAMIC},
		{"radius",		EAP_RADIUS},
	};

	for (i = 0; i < countof(types); i++)
	{
		if (strcaseeq(name, types[i].name))
		{
			return types[i].type;
		}
	}
	return 0;
}
