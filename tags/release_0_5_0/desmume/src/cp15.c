/*  Copyright (C) 2006 yopyop
    yopyop156@ifrance.com
    yopyop156.ifrance.com

    This file is part of DeSmuME

    DeSmuME is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    DeSmuME is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DeSmuME; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdlib.h>

#include "cp15.h"
#include "debug.h"
#include "MMU.h"

armcp15_t *armcp15_new(armcpu_t * c)
{
	armcp15_t *armcp15 = (armcp15_t*)malloc(sizeof(armcp15_t));
	if(!armcp15) return NULL;
	
	armcp15->cpu = c;
	armcp15->IDCode = 0x41049460;
	armcp15->cacheType = 0x0F0D2112;
	armcp15->TCMSize = 0x00140140;
	armcp15->ctrl = 0x00000000;
	armcp15->DCConfig = 0x0;    
	armcp15->ICConfig = 0x0;    
	armcp15->writeBuffCtrl = 0x0;
	armcp15->und = 0x0;
	armcp15->DaccessPerm = 0x22222222;
	armcp15->IaccessPerm = 0x22222222;
	armcp15->protectBaseSize0 = 0x0;
	armcp15->protectBaseSize1 = 0x0;
	armcp15->protectBaseSize2 = 0x0;
	armcp15->protectBaseSize3 = 0x0;
	armcp15->protectBaseSize4 = 0x0;
	armcp15->protectBaseSize5 = 0x0;
	armcp15->protectBaseSize6 = 0x0;
	armcp15->protectBaseSize7 = 0x0;
	armcp15->cacheOp = 0x0;
	armcp15->DcacheLock = 0x0;
	armcp15->IcacheLock = 0x0;
	armcp15->ITCMRegion = 0x0C;
	armcp15->DTCMRegion = 0x0080000A;
	armcp15->processID = 0;
	
	return armcp15;
}


BOOL armcp15_dataProcess(armcp15_t *armcp15, u8 CRd, u8 CRn, u8 CRm, u8 opcode1, u8 opcode2)
{
   return FALSE;
}

BOOL armcp15_load(armcp15_t *armcp15, u8 CRd, u8 adr)
{
   return FALSE;
}

BOOL armcp15_store(armcp15_t *armcp15, u8 CRd, u8 adr)
{
   return FALSE;
}

BOOL armcp15_moveCP2ARM(armcp15_t *armcp15, u32 * R, u8 CRn, u8 CRm, u8 opcode1, u8 opcode2)
{
	if(armcp15->cpu->CPSR.bits.mode == USR) return FALSE;
	
	switch(CRn)
	{
		case 0 :
			if((opcode1 == 0)&&(CRm==0))
			{
			switch(opcode2)
			{
				case 1 :
					*R = armcp15->cacheType;
					return TRUE;
				case 2 :
					*R = armcp15->TCMSize;
					return TRUE;
				default :
					*R = armcp15->IDCode;
					return TRUE;
			}
			}
			return FALSE;
		case 1 :
			if((opcode1==0) && (opcode2==0) && (CRm==0))
			{
				*R = armcp15->ctrl;
				return TRUE;
			}
			return FALSE;
			
		case 2 :
			if((opcode1==0) && (CRm==0))
			{
				switch(opcode2)
				{
					case 0 : 
						*R = armcp15->DCConfig;
					return TRUE;
					case 1 :
						*R = armcp15->ICConfig;
					return TRUE;
					default :
					return FALSE;
				}
			}
			return FALSE;
		case 3 :
			if((opcode1==0) && (opcode2==0) && (CRm==0))
			{
				*R = armcp15->writeBuffCtrl;
				return TRUE;
			}
			return FALSE;
		case 5 :
			if((opcode1==0) && (CRm==0))
			{
				switch(opcode2)
				{
					case 2 : 
						*R = armcp15->DaccessPerm;
					return TRUE;
					case 3 :
						*R = armcp15->IaccessPerm;
					return TRUE;
					default :
					return FALSE;
				}
			}
			return FALSE;
		case 6 :
			if((opcode1==0) && (opcode2==0))
			{
				switch(CRm)
				{
					case 0 : 
						*R = armcp15->protectBaseSize0;
					return TRUE;
					case 1 :
						*R = armcp15->protectBaseSize1;
					return TRUE;
					case 2 :
						*R = armcp15->protectBaseSize2;
					return TRUE;
					case 3 :
						*R = armcp15->protectBaseSize3;
					return TRUE;
					case 4 :
						*R = armcp15->protectBaseSize4;
					return TRUE;
					case 5 :
						*R = armcp15->protectBaseSize5;
					return TRUE;
					case 6 :
						*R = armcp15->protectBaseSize6;
					return TRUE;
					case 7 :
						*R = armcp15->protectBaseSize7;
					return TRUE;
					default :
					return FALSE;
				}
			}
			return FALSE;
		case 9 :
			if((opcode1==0))
			{
				switch(CRm)
				{
					case 0 :
						switch(opcode2)
						{
							case 0 : 
							*R = armcp15->DcacheLock;
							return TRUE;
							case 1 :
							*R = armcp15->IcacheLock;
							return TRUE;
							default :
								return FALSE;
						}
					case 1 :
						switch(opcode2)
						{
						case 0 : 
						*R = armcp15->DTCMRegion;
						return TRUE;
						case 1 :
						*R = armcp15->ITCMRegion;
						return TRUE;
						default :
							return FALSE;
						}
				}
			}
		return FALSE;
		default :
			return FALSE;
	}
}


u32 CP15wait4IRQ(armcpu_t *cpu)
{
	if(cpu->wirq)
	{
		if(!cpu->waitIRQ)
		{
			cpu->waitIRQ = 0;
			cpu->wirq = 0;
			//cpu->switchMode(oldmode[cpu->proc_ID]);
		return 1;
		}
		cpu->R[15] = cpu->instruct_adr;
		cpu->next_instruction = cpu->R[15];
		return 1;
	}
	cpu->waitIRQ = 1;
	cpu->wirq = 1;
	cpu->R[15] = cpu->instruct_adr;
	cpu->next_instruction = cpu->R[15];
	MMU.reg_IME[0] = 1;
	return 1;
}

BOOL armcp15_moveARM2CP(armcp15_t *armcp15, u32 val, u8 CRn, u8 CRm, u8 opcode1, u8 opcode2)
{
	if(armcp15->cpu->CPSR.bits.mode == USR) return FALSE;
	
	switch(CRn)
	{
		case 1 :
		if((opcode1==0) && (opcode2==0) && (CRm==0))
		{
			armcp15->ctrl = val;
			MMU.ARM9_RW_MODE = BIT7(val);
			armcp15->cpu->intVector = 0x0FFF0000 * (BIT13(val));
			armcp15->cpu->LDTBit = !BIT15(val); //TBit
			/*if(BIT17(val))
			{
				log::ajouter("outch !!!!!!!");
			}
			if(BIT19(val))
			{
				log::ajouter("outch !!!!!!!");
			}*/
			return TRUE;
		}
		return FALSE;
		case 2 :
		if((opcode1==0) && (CRm==0))
		{
			switch(opcode2)
			{
				case 0 : 
					armcp15->DCConfig = val;
					return TRUE;
				case 1 :
					armcp15->ICConfig = val;
					return TRUE;
				default :
					return FALSE;
			}
		}
		return FALSE;
		case 3 :
		if((opcode1==0) && (opcode2==0) && (CRm==0))
		{
			armcp15->writeBuffCtrl = val;
			return TRUE;
		}
		return FALSE;
		if((opcode1==0) && (CRm==0))
		{
			switch(opcode2)
			{
				case 2 : 
					armcp15->DaccessPerm = val;
					return TRUE;
				case 3 :
					armcp15->IaccessPerm = val;
					return TRUE;
				default :
					return FALSE;
			}
		}
		return FALSE;
		case 6 :
		if((opcode1==0) && (opcode2==0))
		{
			switch(CRm)
			{
				case 0 : 
					armcp15->protectBaseSize0 = val;
					return TRUE;
				case 1 :
					armcp15->protectBaseSize1 = val;
					return TRUE;
				case 2 :
					armcp15->protectBaseSize2 = val;
					return TRUE;
				case 3 :
					armcp15->protectBaseSize3 = val;
					return TRUE;
				case 4 :
					armcp15->protectBaseSize4 = val;
					return TRUE;
				case 5 :
					armcp15->protectBaseSize5 = val;
					return TRUE;
				case 6 :
					armcp15->protectBaseSize6 = val;
					return TRUE;
				case 7 :
					armcp15->protectBaseSize7 = val;
					return TRUE;
				default :
					return FALSE;
			}
		}
		return FALSE;
		case 7 :
		if((CRm==0)&&(opcode1==0)&&((opcode2==4)))
		{
			CP15wait4IRQ(armcp15->cpu);
			return TRUE;
		}
		return FALSE;
		case 9 :
		if((opcode1==0))
		{
			switch(CRm)
			{
				case 0 :
				switch(opcode2)
				{
					case 0 : 
						armcp15->DcacheLock = val;
						return TRUE;
					case 1 :
						armcp15->IcacheLock = val;
						return TRUE;
					default :
						return FALSE;
			}
			case 1 :
			switch(opcode2)
			{
				case 0 : 
					armcp15->DTCMRegion = val;
					MMU.DTCMRegion = val & 0x0FFFFFFC0;
					/*sprintf(logbuf, "%08X", val);
					log::ajouter(logbuf);*/
					return TRUE;
				case 1 :
					armcp15->ITCMRegion = val;
					MMU.ITCMRegion = val & 0x0FFFFFFC0;
					return TRUE;
				default :
					return FALSE;
				}
			}
		}
		return FALSE;
		default :
			return FALSE;
	}
}


 