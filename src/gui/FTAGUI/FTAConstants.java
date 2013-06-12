/******************************************************************************
** 
** OpenFTA - Fault Tree Analysis
** Copyright (C) 2005 FSC Limited
** 
** This program is free software; you can redistribute it and*or modify it
** under the terms of the GNU General Public License as published by the Free
** Software Foundation; either version 2 of the License, or (at your 
** option) any later version.
** 
** This program is distributed in the hope that it will be useful, but WITHOUT
** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
** FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
** more details.
**
** You should have received a copy of the GNU General Public License along 
** with this program; if not, write to the Free Software Foundation, Inc., 
** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
**
** To contact FSC, please send an email to support@fsc.co.uk or write to 
** FSC Ltd., Cardiff Business Technology Centre, Senghenydd Road, Cardiff,
** CF24 4AY.
**
******************************************************************************/

package FTAGUI;

import java.awt.*;
import java.util.*;

public class FTAConstants{

    static final int
	UNKNOWN       = '?',
	BASIC         = 'B',
	AND           = 'A',
	OR            = 'O',
	TRANSIN       = 'I',
	TRANSOUT      = 'T',
	INTERMEDIATE  = 'M',
	UNDEVELOP     = 'U',
	INHIBIT       = 'H',
	PRIORITY_AND  = 'P',
	XOR           = 'X',
	EXTERNAL      = 'E',
	COND_NOT_ANAL = 'N',
	COND_ANAL     = 'C',
	DORMANT       = 'D',

	INTERMEDIATE_POS = 0,
	BASIC_POS        = 1,
	UNDEVELOP_POS    = 2,
	EXTERNAL_POS     = 3,
	COND_POS         = 4,
	AND_POS          = 5,
	OR_POS           = 6,
	PRIORITY_AND_POS = 7,
	XOR_POS          = 8,
	INHIBIT_POS      = 9,
	TRANSIN_POS      = 10,
	TRANSOUT_POS     = 11,

	ANALYSIS_VALIDATE_POS      = 0,
	ANALYSIS_CUT_SET_POS       = 1,
	ANALYSIS_PROBABILITIES_POS = 2,	
	ANALYSIS_MONTE_CARLO_POS   = 3,

	VIEW_QUALITATIVE_POS = 0,
	VIEW_PROBABILITY_POS = 1,	
	VIEW_MONTE_CARLO_POS = 2,

	PRINT_QUALITATIVE_POS = 0,
	PRINT_PROBABILITY_POS = 1,	
	PRINT_MONTE_CARLO_POS = 2,

	SYMB_BASIC = 0,
	SYMB_UNDEV = 1,
	SYMB_INTERMEDIATE = 2, 
	SYMB_AND = 3, 
	SYMB_OR = 4,
	SYMB_PRIORITY_AND = 5, 
	SYMB_TRANS_IN = 6,
	SYMB_CONDITION = 7,
	SYMB_EXTERN = 8,
	SYMB_EXCL_OR = 9, 
	SYMB_INHIBIT= 10, 
	SYMB_TRANS_OUT = 11, 
	SYMB_INVALID = 12;

}
