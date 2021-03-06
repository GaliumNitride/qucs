/* -*-c-*- */

%{
/*
 * scan_spice.l - scanner for a Spice netlist
 *
 * Copyright (C) 2004-2009 Stefan Jahn <stefan@lkcc.org>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $Id$
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef __MINGW32__
#include <io.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "check_spice.h"
#include "tokens_spice.h"

#if !HAVE_STRCHR
# define strchr  index
# define strrchr rindex
#endif

/* fixing invalid identifiers */
static void spice_fix_identifier (char * ident) {
  char * p;
  for (p = ident; *p != '\0'; p++)
    switch (*p) {
    case '+': *p = 'P'; break;
    case '-': *p = 'N'; break;
    case '/': *p = '_'; break;
    }
}

%}

WS       [ \t\n\r]
TITLE    [* \t0-9A-Za-z][A-Za-z0-9\- \t;#:=()/\.,*\\]*\r*\n
SPACE    [ \t]
NAME     [A-Z][A-Z0-9]*
IDENT    [A-Za-z_][A-Za-z0-9_\-/]*
IDENT2   [A-Za-z0-9_][A-Za-z0-9_\-/]*
FILE     [/A-Za-z0-9_][/\\:A-Za-z0-9_\.]*
DIGIT    [0-9]
EXPONENT [Ee][+-]?{DIGIT}+
INT      [+-]?{DIGIT}+
FLOAT1   [+-]?{DIGIT}+{EXPONENT}
FLOAT2   [+-]?{DIGIT}*"."{DIGIT}+({EXPONENT})?
FLOAT3   [+-]?{DIGIT}+"."
NUMBER   ({INT}|{FLOAT1}|{FLOAT2}|{FLOAT3}){SFX}?{UNIT}?
SFX      ([tT]|[gG]|[mM]|[kK]|[uU]|[nN]|[pP]|[fF]|[mM][iI][lL]|[mM][eE][gG])
UNIT     ([sS]|[vV]|[aA]|[oO][hH][mM]|[mM][hH][oO]|[fF]|[hH])
NODE     ([A-Za-z0-9_#\+-]+|[\+]|[-])
EOL      \r*\n
CSFX     [A-Za-z0-9_]*

     /* Source functions */
FUN1     ([dD][eE][cC]|[lL][iI][nN]|[oO][cC][tT]|[dD][cC]|[aA][cC])
FUN2     ([sS][iI][nN]|[pP][uU][lL][sS][eE]|[pP][wW][lL]|[eE][xX][pP])
FUN3     ([dD][iI][sS][tT][oO][fF][12])
FUNCTION ({FUN1}|{FUN2}|{FUN3})

     /* Device Models */
MODEL1   ([rR]|[cC]|[sS][wW]|[cC][sS][wW]|[uU][rR][cC]|[lL][tT][rR][aA])
MODEL2   ([dD]|[nN][pP][nN]|[pP][nN][pP]|[nN][jJ][fF]|[pP][jJ][fF])
MODEL3   ([nN][mM][oO][sS]|[pP][mM][oO][sS]|[nN][mM][fF]|[pP][mM][fF])
MODEL4   ([rR][eE][sS]|[vV][sS][wW][iI][tT][cC][hH])
MODEL    ({MODEL1}|{MODEL2}|{MODEL3}|{MODEL4})

     /* Voltage derivatives */
VFUN     ([vV]|[vV][rR]|[vV][iI]|[vV][mM]|[vV][pP]|[vV][dD][bB])

     /* Options specials */
OPT1     ([aA][cC][cC][tT]|[lL][iI][sS][tT]|[nN][oO][mM][oO][dD])
OPT2     ([nN][oO][pP][aA][gG][eE]|[nN][oO][dD][eE]|[oO][pP][tT][sS])
OPT3     ([tT][rR][yY][tT][oO][cC][oO][mM][pP][aA][cC][tT])
OPT4     ([kK][eE][eE][pP][oO][pP][iI][nN][fF][oO]|[bB][aA][dD][mM][oO][sS]3)
OPT5     ([pP][oO][sS][tT]|[tT][rR][aA][nN][sS])
OPTS     ({OPT1}|{OPT2}|{OPT3}|{OPT4}|{OPT5})

     /* Model specific properties */
MOD1     ([nN][oO][cC][oO][nN][tT][rR][oO][lL])
MOD2     ([sS][tT][eT][pP][lL][iI][mM][iI][tT])
MOD3     ([qQ][uU][aA][dD][iI][nN][tT][eE][rR][pP])
MOD4     ([nN][oO][pP][rR][iI][nN][tT])
MODS     ({MOD1}|{MOD2}|{MOD3}|{MOD4})

     /* Plot/Print specific identifiers */
SIM1     ([aA][cC]|[dD][cC]|[tT][rR][aA][nN]|[nN][oO][iI][sS][eE])
SIM2     ([dD][iI][sS][tT][oO]|[pP][zZ])
SIMS     ({SIM1}|{SIM2})

     /* MOS specific device properties */
MOS1     ([lL]|[wW]|[aA][dDsS]|[pP][dDsS]|[nN][rR][dDsS]|[dD][eE][bB][uU][gG])
MOS2     ([rR][gG][eE][oO][mM][oO][dD]|[tT][sSdD]|[mM])
MOS      ({MOS1}|{MOS2})

     /* analog behavioural */
TABLE    [tT][aA][bB][lL][eE]
VALUE    [vV][aA][lL][uU][eE]
FREQ     [fF][rR][eE][qQ]
LAPLACE  [lL][aA][pP][lL][aA][cC][eE]
POLY     [pP][oO][lL][yY]

%x COMMENT IVREF DEVPROP LREF MODREF1 MODREF2 IGNORE FUNREF FILEREF VREF
%x STARTUP VSINGLE ISWITCH VSWITCH CONTROL GEVALS INLINE SUBCKT TLPROP RLCPROP
%x FHVALS
%option yylineno noyywrap nounput prefix="spice_"

%%

<INITIAL>^{TITLE} { /* detect initial title lines */
    spice_lval.str = strdup (spice_text);
    BEGIN(STARTUP);
    return TitleLine;
}

<STARTUP,INITIAL>^{SPACE}+ { /* Spice3 ignores any lines starting spaces */
    BEGIN(COMMENT);
  }

<STARTUP,INITIAL>^"*" { /* ignored region begins here */
    BEGIN(COMMENT);
  }

<STARTUP,INITIAL,IVREF,DEVPROP,LREF,MODREF1,MODREF2,IGNORE,FUNREF,
 FILEREF,VREF,VSINGLE,ISWITCH,VSWITCH,CONTROL,GEVALS,INLINE,SUBCKT,
 TLPROP,RLCPROP,FHVALS>[\*\$] {
    /* ignored inline region begins here (3f5, 2g6 and hspice) */
    BEGIN(INLINE);
  }

<STARTUP,INITIAL>^"."[cC][oO][nN][tT][rR][oO][lL] {
    /* control (interactive) region start */
    BEGIN(CONTROL);
}

<CONTROL>.     { /* skip any character in here */ }
<CONTROL>{EOL} { /* skip any character in here */ }

<CONTROL>^"."[eE][nN][dD][cC] {
    BEGIN(STARTUP);
}

<STARTUP,INITIAL>\x1a {
  /* skip EOF character */
}

<STARTUP,INITIAL>^"."[sS][uU][bB][cC][kK][tT] {
    /* subcircuit definition start */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(SUBCKT);
    return SUBCKT_Action;
}

<STARTUP>^"."[eE][nN][dD][sS] { /* subcircuit definition end */
    BEGIN(SUBCKT);
    return ENDS_Action;
}

<STARTUP,INITIAL>^[rRlLcC]{CSFX} { /* R, L and C instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(RLCPROP);
    return RLC_Device;
}

<STARTUP,INITIAL>^[kK]{CSFX} { /* Mutual inductor instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(LREF);
    return K_Device;
}

<STARTUP,INITIAL>^[iIvV]{CSFX} {
    /* voltage and current source instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(FUNREF);
    return IV_Source;
}

<STARTUP,INITIAL>^[gGeE]{CSFX} {
    /* voltage controlled source instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(GEVALS);
    return GE_Source;
}

<STARTUP,INITIAL>^[fFhH]{CSFX} {
    /* current controlled source instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(FHVALS);
    return FH_Source;
}

<STARTUP,INITIAL>^[bB]{CSFX} {
    /* non-linear dependent source instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(IGNORE);
    return B_Source;
}

<STARTUP,INITIAL>^[xX]{CSFX} { /* subcircuit instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(STARTUP);
    return X_Device;
}

<STARTUP,INITIAL>^[oO]{CSFX} { /* lossy transline */
    spice_lval.ident = strdup (spice_text);
    BEGIN(STARTUP);
    return O_Device;
}

<STARTUP,INITIAL>^[tT]{CSFX} { /* lossless transline */
    spice_lval.ident = strdup (spice_text);
    BEGIN(TLPROP);
    return T_Device;
}

<STARTUP,INITIAL>^[uU]{CSFX} { /* distributed lossy transline */
    spice_lval.ident = strdup (spice_text);
    BEGIN(STARTUP);
    return U_Device;
}

<STARTUP,INITIAL>^[sS]{CSFX} { /* voltage controlled switch */
    spice_lval.ident = strdup (spice_text);
    BEGIN(VSWITCH);
    return S_Device;
}

<STARTUP,INITIAL>^[wW]{CSFX} { /* current controlled switch */
    spice_lval.ident = strdup (spice_text);
    BEGIN(ISWITCH);
    return W_Device;
}

<STARTUP,INITIAL>^"."[mM][oO][dD][eE][lL] { /* device Model definitions */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(MODREF1);
    return MODEL_Action;
}

<STARTUP,INITIAL>^"."[tT][rR][aA][nN] { /* transient analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(STARTUP);
    return TRAN_Action;
}

<STARTUP,INITIAL>^"."[pP][lL][oO][tT] { /* plotting action */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(IVREF);
    return PLOT_Action;
}

<STARTUP,INITIAL>^"."[aA][cC] { /* AC analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(FUNREF);
    return AC_Action;
}

<STARTUP,INITIAL>^"."[dD][iI][sS][tT][oO] { /* distortion analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(FUNREF);
    return DISTO_Action;
}

<STARTUP,INITIAL>^"."[nN][oO][dD][eE][sS][eE][tT] { /* nodeset functionality */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(VSINGLE);
    return NODESET_Action;
}

<STARTUP,INITIAL>^"."[iI][cC] { /* nodeset functionality */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(VSINGLE);
    return NODESET_Action;
}

<STARTUP,INITIAL>^"."[dD][cC] { /* DC analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(IVREF);
    return DC_Action;
}

<STARTUP,INITIAL>^"."[oO][pP] { /* operating point analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(STARTUP);
    return OP_Action;
}

<STARTUP,INITIAL>^"."[tT][eE][mM][pP] { /* temperature analysis (Spice 2g6) */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(STARTUP);
    return TEMP_Action;
}

<STARTUP,INITIAL>^"."[pP][rR][iI][nN][tT] { /* printing action */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(VREF);
    return PRINT_Action;
}

<STARTUP,INITIAL>^"."[oO][pP][tT][iI][oO][nN][sS] { /* general options */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(STARTUP);
    return OPTIONS_Action;
}

<STARTUP,INITIAL>^"."[oO][pP][tT] { /* general options (abbrev.) */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(STARTUP);
    return OPTIONS_Action;
}

<STARTUP,INITIAL>^"."[oO][pP][tT][iI][oO][nN] { /* general options (abbrev.) */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(STARTUP);
    return OPTIONS_Action;
}

<STARTUP,INITIAL>^"."[wW][iI][dD][tT][hH] { /* special width of ??? */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(STARTUP);
    return WIDTH_Action;
}

<STARTUP,INITIAL>^"."[nN][oO][iI][sS][eE] { /* noise analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(IVREF);
    return NOISE_Action;
}

<STARTUP,INITIAL>^"."[iI][nN][cC][lL][uU][dD][eE] { /* file include */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(FILEREF);
    return INCLUDE_Action;
}

<STARTUP,INITIAL>^"."[pP][zZ] { /* pole-zero analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(STARTUP);
    return PZ_Action;
}

<STARTUP,INITIAL>^"."[sS][aA][vV][eE] { /* save line */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(IGNORE);
    return SAVE_Action;
}

<STARTUP,INITIAL>^"."[sS][eE][nN][sS] { /* sensitivity analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(IGNORE);
    return SENS_Action;
}

<STARTUP,INITIAL>^"."[tT][fF] { /* transfer function analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(IGNORE);
    return TF_Action;
}

<STARTUP,INITIAL>^"."[fF][oO][uU][rR] { /* fourier analysis */
    spice_lval.ident = strdup (&spice_text[1]);
    BEGIN(IGNORE);
    return FOUR_Action;
}

<STARTUP>[cC][uU][rR]|[vV][oO][lL] { /* pole-zero specials */
    spice_lval.ident = strdup (spice_text);
    return CurVol;
}

<STARTUP>[pP][oO][lL]|[zZ][eE][rR]|[pP][zZ] { /* other pole-zero specials */
    spice_lval.ident = strdup (spice_text);
    return PoleZero;
}

<FILEREF>{FILE} { /* a file reference */
    spice_lval.ident = strdup (spice_text);
    return File;
}

<IVREF,VREF>[aA][lL][lL] { /* a print special */
    spice_lval.ident = strdup (spice_text);
    return ALL_Special;
}

<RLCPROP>[tT][cC] { /* a TC special */
    spice_lval.ident = strdup (spice_text);
    return TC_Special;
}

<DEVPROP,VSWITCH,ISWITCH>[oO][fF][fF] { /* a device special */
    spice_lval.ident = strdup (spice_text);
    return OFF_Special;
}

<VSWITCH,ISWITCH>[oO][nN] { /* a device special */
    spice_lval.ident = strdup (spice_text);
    return ON_Special;
}

<TLPROP,DEVPROP>[iI][cC] { /* another device special */
    spice_lval.ident = strdup (spice_text);
    return IC_Special;
}

<DEVPROP>[tT][eE][mM][pP] { /* another device special */
    spice_lval.ident = strdup (spice_text);
    return TEMP_Special;
}

<DEVPROP>{MOS} { /* another device special */
    spice_lval.ident = strdup (spice_text);
    return MOS_Special;
}

<IVREF,VREF>{VFUN} { /* voltage derivatives (plotting and printing) */
    spice_lval.ident = strdup (spice_text);
    return VoltFunc;
}

<VSINGLE>[vV] { /* voltage derivatives (plotting and printing) */
    spice_lval.ident = strdup (spice_text);
    return VoltFunc;
}

<STARTUP>{OPTS} { /* option specials */
    spice_lval.ident = strdup (spice_text);
    return Options;
}

<STARTUP>{MODS} { /* Model specials */
    spice_lval.ident = strdup (spice_text);
    return ModelProps;
}

<IVREF,VREF,ISWITCH,FHVALS>[vV]{CSFX} { /* voltage source references */
    spice_lval.ident = strdup (spice_text);
    return V_Source;
}

<IVREF>[iI]{CSFX} { /* current source references */
    spice_lval.ident = strdup (spice_text);
    return I_Source;
}

<IVREF,VREF>[iI] { /* I function (plotting and printing) */
    spice_lval.ident = strdup (spice_text);
    return CurrFunc;
}

<IVREF,VREF>{SIMS} { /* print/plot specific identifiers */
    spice_lval.ident = strdup (spice_text);
    return SIM_Type;
}

<LREF>[lL]{CSFX} { /* inductor references */
    spice_lval.ident = strdup (spice_text);
    return L_Device;
}

<STARTUP>^[dD]{CSFX} { /* diode instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(DEVPROP);
    return Diode_Device;
}

<STARTUP>^[qQ]{CSFX} { /* BJT instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(DEVPROP);
    return Bipolar_Device;
}

<STARTUP>^[jJ]{CSFX} { /* JFET instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(DEVPROP);
    return JFET_Device;
}

<STARTUP>^[mM]{CSFX} { /* MOSFET instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(DEVPROP);
    return MOSFET_Device;
}

<STARTUP>^[zZ]{CSFX} { /* MESFET instances */
    spice_lval.ident = strdup (spice_text);
    BEGIN(DEVPROP);
    return MESFET_Device;
}

<STARTUP,INITIAL>"."[Ee][Nn][Dd]{SPACE}*{EOL}* { /* identify end of netlist */
    return End;
}

<STARTUP,IVREF,DEVPROP,LREF,FUNREF,VREF,VSINGLE,ISWITCH,VSWITCH,
 GEVALS,TLPROP,RLCPROP,FHVALS>{DIGIT}+ {
    /* identify node (containing digits) */
    spice_lval.ident = strdup (spice_text);
    return Digits;
}

<STARTUP,IVREF,DEVPROP,LREF,FUNREF,VREF,VSINGLE,GEVALS,TLPROP,
 RLCPROP,FHVALS>{NUMBER} {
    /* identify float (any kind) */
    spice_lval.ident = strdup (spice_text);
    return Floats;
}

<GEVALS,FHVALS,RLCPROP>{POLY} {
    /* identify analog behaviour */
    spice_lval.ident = strdup (spice_text);
    return Behave;
}

<GEVALS,FHVALS>{TABLE}|{VALUE}|{FREQ}|{LAPLACE} {
    /* identify analog behaviour */
    spice_lval.ident = strdup (spice_text);
    BEGIN(IGNORE);
    return Behave;
}

<IVREF,FUNREF>{FUNCTION} { /* function specials (e.g. in sources) */
    spice_lval.ident = strdup (spice_text);
    return Function;
}

<MODREF2>{MODEL} { /* Model specification specials */
    spice_lval.ident = strdup (spice_text);
    BEGIN(STARTUP);
    return MODEL_Spec;
}

<STARTUP,DEVPROP,MODREF2,ISWITCH,VSWITCH,
 TLPROP,RLCPROP>{IDENT} { /* arbitrary identifier */
    spice_lval.ident = strdup (spice_text);
    spice_fix_identifier (spice_lval.ident);
    return Identifier;
}

<SUBCKT>{IDENT2} { /* arbitrary identifier */
    spice_lval.ident = strdup (spice_text);
    spice_fix_identifier (spice_lval.ident);
    BEGIN(STARTUP);
    return Identifier;
}

<MODREF1>{IDENT2} { /* arbitrary identifier */
    spice_lval.ident = strdup (spice_text);
    spice_fix_identifier (spice_lval.ident);
    BEGIN(MODREF2);
    return Identifier;
}

<IVREF,VREF>[vV]{CSFX}#[bB][rR][aA][nN][cC][hH] {
    /* I function (plotting and printing) */
    spice_lval.ident = strdup (spice_text);
    return BranchFunc;
}

<IVREF,VREF>@[a-zA-Z]{CSFX}\[[a-zA-Z]{CSFX}\] {
    /* operating point (plotting and printing) */
    spice_lval.ident = strdup (spice_text);
    return OpFunc;
}

<STARTUP,IVREF,DEVPROP,FUNREF,VREF,VSINGLE,ISWITCH,VSWITCH,GEVALS,
 TLPROP,RLCPROP,FHVALS>{NODE} {
    /* identify node */
    spice_lval.ident = strdup (spice_text);
    return Nodes;
}

<STARTUP,IVREF,DEVPROP,LREF,IGNORE,FUNREF,FILEREF,VREF,VSINGLE,
 ISWITCH,VSWITCH,GEVALS,SUBCKT,TLPROP,RLCPROP,FHVALS>{EOL} {
    /* detect end of line */
    BEGIN(STARTUP);
    return Eol;
}

<*>{SPACE}|{EOL}+\+|[()=,] {
    /* skip spaces, '=', '(', ')', ',' and the leading '+' */
}

<COMMENT>{EOL} { BEGIN(STARTUP); /* skipping ends here */ }
<COMMENT>. { /* skip any character in here */ }
<IGNORE>.  { /* skip any character in here */ }

<INLINE>{EOL} { BEGIN(STARTUP); return Eol; /* skipping ends here */ }
<INLINE>.  { /* skip any character in here */ }
<*>.       { /* skip any character in here */ }

<*>. { /* any other character is invalid */
    fprintf (stderr,
	     "line %d: syntax error, unrecognized character: `%s'\n",
	     spice_lineno, spice_text);
    return InvalidCharacter;
}

%%
