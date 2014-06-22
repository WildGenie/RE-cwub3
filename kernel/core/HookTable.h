#pragma once

const unsigned char HookTable[] =
{
	0x5D, 0x00, 0x00, 0x80, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x03, 0xD3, 0x25, 0x77, 0xD4, 0x3C, 0x5C, 0xE6, 0xCB, 0x45, 
	0x7C, 0x64, 0xB8, 0x80, 0x43, 0x3E, 0x38, 0xF9, 0xDB, 0x09, 0x55, 0x8A, 0xE9, 
	0x51, 0x91, 0x56, 0x6F, 0x68, 0xB6, 0xB4, 0x79, 0x30, 0x99, 0x66, 0x93, 0x40, 
	0x58, 0x3A, 0x08, 0xD6, 0x49, 0xAA, 0x1C, 0x32, 0x89, 0xB8, 0xFB, 0x08, 0x10, 
	0x9B, 0xF3, 0x0A, 0x18, 0x5B, 0xE2, 0xCF, 0xD1, 0xCF, 0x26, 0x0C, 0x47, 0x41, 
	0x72, 0x1B, 0xBB, 0xE4, 0xDB, 0x3E, 0xB5, 0x34, 0xB0, 0xA7, 0xD2, 0x83, 0x00, 
	0x75, 0xC0, 0xE2, 0x92, 0xDE, 0x4D, 0x8A, 0xCD, 0x85, 0x27, 0xE6, 0x3E, 0x2B, 
	0x3F, 0x98, 0x36, 0x74, 0x43, 0x9C, 0x19, 0x94, 0x68, 0x47, 0xFD, 0x35, 0xA4, 
	0x66, 0x54, 0x40, 0xF7, 0x47, 0x4F, 0xA2, 0x9F, 0xC3, 0x91, 0xDB, 0xCE, 0xD2, 
	0x6F, 0xFF, 0x5C, 0xC3, 0xF2, 0x05, 0x46, 0xE9, 0x7F, 0x78, 0x56, 0x76, 0xB3, 
	0x25, 0x92, 0xA6, 0x7D, 0xDF, 0xD8, 0x7F, 0x13, 0xBD, 0xF4, 0x1E, 0xD3, 0x31, 
	0x41, 0xD1, 0x80, 0xE3, 0xD2, 0x59, 0x1A, 0x82, 0xC5, 0x2E, 0xF4, 0x88, 0xE8, 
	0x5A, 0x09, 0xF1, 0x9A, 0xB8, 0x2B, 0x99, 0xAD, 0x43, 0x85, 0x79, 0xED, 0x2A, 
	0xB4, 0xAF, 0x5B, 0x1A, 0x36, 0x4B, 0x6C, 0x03, 0xF5, 0x4A, 0xFC, 0x62, 0xDC, 
	0xF4, 0x1A, 0x0D, 0xC6, 0x28, 0x5B, 0x0B, 0x84, 0xA6, 0xFB, 0x34, 0x76, 0x7B, 
	0x6A, 0xF6, 0x4C, 0x6D, 0x04, 0x13, 0x06, 0x87, 0xB0, 0xEA, 0xEA, 0xFA, 0x1C, 
	0x79, 0xCA, 0xD8, 0x28, 0x09, 
};

#define C_ERROR   0xFFFFFFFF
#define C_ADDR1   0x00000001
#define C_ADDR2   0x00000002
#define C_ADDR4   0x00000004
#define C_LOCK    0x00000008
#define C_67      0x00000010
#define C_66      0x00000020
#define C_REP     0x00000040
#define C_SEG     0x00000080
#define C_ANYPREFIX  (C_66+C_67+C_LOCK+C_REP+C_SEG)
#define C_DATA1   0x00000100
#define C_DATA2   0x00000200
#define C_DATA4   0x00000400
#define C_SIB     0x00000800
#define C_ADDR67  0x00001000
#define C_DATA66  0x00002000
#define C_MODRM   0x00004000
#define C_BAD     0x00008000
#define C_OPCODE2 0x00010000
#define C_REL     0x00020000
#define C_STOP    0x00040000

struct disasm_struct
{
  BYTE  disasm_defaddr;         // 00
  BYTE  disasm_defdata;         // 01
  DWORD disasm_len;             // 02 03 04 05
  DWORD disasm_flag;            // 06 07 08 09
  DWORD disasm_addrsize;        // 0A 0B 0C 0D
  DWORD disasm_datasize;        // 0E 0F 10 11
  BYTE  disasm_rep;             // 12
  BYTE  disasm_seg;             // 13
  BYTE  disasm_opcode;          // 14
  BYTE  disasm_opcode2;         // 15
  BYTE  disasm_modrm;           // 16
  BYTE  disasm_sib;             // 17
  union
  {
  BYTE  disasm_addr_b[8];       // 18 19 1A 1B  1C 1D 1E 1F
  WORD  disasm_addr_w[4];
  DWORD disasm_addr_d[2];
  char  disasm_addr_c[8];
  short disasm_addr_s[4];
  long  disasm_addr_l[2];
  };
  union
  {
  BYTE  disasm_data_b[8];       // 20 21 22 23  24 25 26 27
  WORD  disasm_data_w[4];
  DWORD disasm_data_d[2];
  char  disasm_data_c[8];
  short disasm_data_s[4];
  long  disasm_data_l[2];
  };
}; // disasm_struct

DWORD *tables;

int disasm(BYTE* opcode0, disasm_struct* diza)
{
  BYTE* opcode = opcode0;

  memset(diza, 0x00, sizeof(disasm_struct));
  diza->disasm_defdata = 4;
  diza->disasm_defaddr = 4;

  if (*(WORD*)opcode == 0x0000) return 0;
  if (*(WORD*)opcode == 0xFFFF) return 0;

  DWORD flag = 0;

repeat_prefix:

  BYTE c = *opcode++;

  DWORD t = tables[ c ];

  if (t & C_ANYPREFIX)
  {

    if (flag & t) return 0;    // twice LOCK,SEG,REP,66,67

    flag |= t;

    if (t & C_67)
    {
      diza->disasm_defaddr ^= 2^4;
    }
    else
    if (t & C_66)
    {
      diza->disasm_defdata ^= 2^4;
    }
    else
    if (t & C_SEG)
    {
      diza->disasm_seg = c;
    }
    else
    if (t & C_REP)
    {
      diza->disasm_rep = c;
    }
    // LOCK

    goto repeat_prefix;

  } // C_ANYPREFIX

  flag |= t;

  diza->disasm_opcode = c;

  if (c == 0x0F)
  {
    c = *opcode++;

    diza->disasm_opcode2 = c;

    flag |= tables[ 256 + c ]; // 2nd flagtable half

    if (flag == C_ERROR) return 0;
  }
  else
  if (c == 0xF7)
  {
    if (((*opcode) & 0x38)==0)
      flag |= C_DATA66;
  }
  else
  if (c == 0xF6)
  {
    if (((*opcode) & 0x38)==0)
      flag |= C_DATA1;
  }
  else
  if (c == 0xCD)
  {
    if (*opcode == 0x20)
      flag |= C_DATA4;
    else
      flag |= C_DATA1;
  }

  if (flag & C_MODRM)
  {
    c = *opcode++;

    diza->disasm_modrm = c;

    if ((c & 0x38) == 0x20)
    if (diza->disasm_opcode == 0xFF)
      flag |= C_STOP;

    BYTE mod = c & 0xC0;
    BYTE rm  = c & 0x07;

    if (mod != 0xC0)
    {
      if (diza->disasm_defaddr == 4)
      {
        if (rm == 4)
        {
          flag |= C_SIB;
          c = *opcode++;
          diza->disasm_sib = c;
          rm = c & 0x07;
        }

        if (mod == 0x40)
        {
          flag |= C_ADDR1;
        }
        else
        if (mod == 0x80)
        {
          flag |= C_ADDR4;
        }
        else
        {
          if (rm == 5)
            flag |= C_ADDR4;
        }
      }
      else // MODRM 16-bit
      {

        if (mod == 0x40)
        {
          flag |= C_ADDR1;
        }
        else
        if (mod == 0x80)
        {
          flag |= C_ADDR2;
        }
        else
        {
          if (rm == 6)
            flag |= C_ADDR2;
        }
      }
    }
  } // C_MODRM

  diza->disasm_flag = flag;

  DWORD a =  flag & (C_ADDR1 | C_ADDR2 | C_ADDR4);
  DWORD d = (flag & (C_DATA1 | C_DATA2 | C_DATA4)) >> 8;

  if (flag & C_ADDR67) a += diza->disasm_defaddr;
  if (flag & C_DATA66) d += diza->disasm_defdata;

  diza->disasm_addrsize = a;
  diza->disasm_datasize = d;

  for(DWORD i=0; i<a; i++)
    diza->disasm_addr_b[i] = *opcode++;

  for(DWORD j=0; j<d; j++)
    diza->disasm_data_b[j] = *opcode++;

  diza->disasm_len = opcode - opcode0;

  return diza->disasm_len;

}

int oplen(BYTE *opcode){
        disasm_struct diza;
        memset(&diza,0,sizeof(diza));

        disasm((BYTE*)opcode,&diza);
        if((diza.disasm_flag == C_ERROR) || ((diza.disasm_flag&C_STOP) == C_STOP) || ((diza.disasm_flag&C_REL)==C_REL) || ((diza.disasm_flag&C_BAD)==C_BAD))
        //if((diza.disasm_flag == C_ERROR))
                return -1;

        return diza.disasm_len;
}