#pragma once

#include "Emu/Cell/SPUOpcodes.h"
#include "Utilities/SharedMutex.h"

#include <set>

class SPUThread;

// Type of the runtime functions generated by SPU recompiler
using spu_jit_func_t = u32(*)(SPUThread* _spu, be_t<u32>* _ls);

// SPU Instruction Classifier
namespace spu_itype
{
	struct type
	{
		u32 UNK;
		u32 STOP;
		u32 LNOP;
		u32 SYNC;
		u32 DSYNC;
		u32 MFSPR;
		u32 RDCH;
		u32 RCHCNT;
		u32 SF;
		u32 OR;
		u32 BG;
		u32 SFH;
		u32 NOR;
		u32 ABSDB;
		u32 ROT;
		u32 ROTM;
		u32 ROTMA;
		u32 SHL;
		u32 ROTH;
		u32 ROTHM;
		u32 ROTMAH;
		u32 SHLH;
		u32 ROTI;
		u32 ROTMI;
		u32 ROTMAI;
		u32 SHLI;
		u32 ROTHI;
		u32 ROTHMI;
		u32 ROTMAHI;
		u32 SHLHI;
		u32 A;
		u32 AND;
		u32 CG;
		u32 AH;
		u32 NAND;
		u32 AVGB;
		u32 MTSPR;
		u32 WRCH;
		u32 BIZ;
		u32 BINZ;
		u32 BIHZ;
		u32 BIHNZ;
		u32 STOPD;
		u32 STQX;
		u32 BI;
		u32 BISL;
		u32 IRET;
		u32 BISLED;
		u32 HBR;
		u32 GB;
		u32 GBH;
		u32 GBB;
		u32 FSM;
		u32 FSMH;
		u32 FSMB;
		u32 FREST;
		u32 FRSQEST;
		u32 LQX;
		u32 ROTQBYBI;
		u32 ROTQMBYBI;
		u32 SHLQBYBI;
		u32 CBX;
		u32 CHX;
		u32 CWX;
		u32 CDX;
		u32 ROTQBI;
		u32 ROTQMBI;
		u32 SHLQBI;
		u32 ROTQBY;
		u32 ROTQMBY;
		u32 SHLQBY;
		u32 ORX;
		u32 CBD;
		u32 CHD;
		u32 CWD;
		u32 CDD;
		u32 ROTQBII;
		u32 ROTQMBII;
		u32 SHLQBII;
		u32 ROTQBYI;
		u32 ROTQMBYI;
		u32 SHLQBYI;
		u32 NOP;
		u32 CGT;
		u32 XOR;
		u32 CGTH;
		u32 EQV;
		u32 CGTB;
		u32 SUMB;
		u32 HGT;
		u32 CLZ;
		u32 XSWD;
		u32 XSHW;
		u32 CNTB;
		u32 XSBH;
		u32 CLGT;
		u32 ANDC;
		u32 FCGT;
		u32 DFCGT;
		u32 FA;
		u32 FS;
		u32 FM;
		u32 CLGTH;
		u32 ORC;
		u32 FCMGT;
		u32 DFCMGT;
		u32 DFA;
		u32 DFS;
		u32 DFM;
		u32 CLGTB;
		u32 HLGT;
		u32 DFMA;
		u32 DFMS;
		u32 DFNMS;
		u32 DFNMA;
		u32 CEQ;
		u32 MPYHHU;
		u32 ADDX;
		u32 SFX;
		u32 CGX;
		u32 BGX;
		u32 MPYHHA;
		u32 MPYHHAU;
		u32 FSCRRD;
		u32 FESD;
		u32 FRDS;
		u32 FSCRWR;
		u32 DFTSV;
		u32 FCEQ;
		u32 DFCEQ;
		u32 MPY;
		u32 MPYH;
		u32 MPYHH;
		u32 MPYS;
		u32 CEQH;
		u32 FCMEQ;
		u32 DFCMEQ;
		u32 MPYU;
		u32 CEQB;
		u32 FI;
		u32 HEQ;
		u32 CFLTS;
		u32 CFLTU;
		u32 CSFLT;
		u32 CUFLT;
		u32 BRZ;
		u32 STQA;
		u32 BRNZ;
		u32 BRHZ;
		u32 BRHNZ;
		u32 STQR;
		u32 BRA;
		u32 LQA;
		u32 BRASL;
		u32 BR;
		u32 FSMBI;
		u32 BRSL;
		u32 LQR;
		u32 IL;
		u32 ILHU;
		u32 ILH;
		u32 IOHL;
		u32 ORI;
		u32 ORHI;
		u32 ORBI;
		u32 SFI;
		u32 SFHI;
		u32 ANDI;
		u32 ANDHI;
		u32 ANDBI;
		u32 AI;
		u32 AHI;
		u32 STQD;
		u32 LQD;
		u32 XORI;
		u32 XORHI;
		u32 XORBI;
		u32 CGTI;
		u32 CGTHI;
		u32 CGTBI;
		u32 HGTI;
		u32 CLGTI;
		u32 CLGTHI;
		u32 CLGTBI;
		u32 HLGTI;
		u32 MPYI;
		u32 MPYUI;
		u32 CEQI;
		u32 CEQHI;
		u32 CEQBI;
		u32 HEQI;
		u32 HBRA;
		u32 HBRR;
		u32 ILA;
		u32 SELB;
		u32 SHUFB;
		u32 MPYA;
		u32 FNMS;
		u32 FMA;
		u32 FMS;
	};
};

// SPU basic function information structure
struct spu_function_t
{
	// Entry point (LS address)
	const u32 addr;

	// Function size (in bytes)
	const u32 size;

	// Function contents (binary copy)
	std::vector<be_t<u32>> data;

	// Basic blocks (start addresses)
	std::set<u32> blocks;

	// Functions possibly called by this function (may not be available)
	std::set<u32> adjacent;

	// Jump table values (start addresses)
	std::set<u32> jtable;

	// Whether ila $SP,* instruction found
	bool does_reset_stack;

	// Pointer to the compiled function
	spu_jit_func_t compiled = nullptr;

	spu_function_t(u32 addr, u32 size)
		: addr(addr)
		, size(size)
	{
	}
};

// SPU Function Database (must be global or PS3 process-local)
class SPUDatabase final
{
	shared_mutex m_mutex;

	// All registered functions (uses addr and first instruction as a key)
	std::unordered_multimap<u64, std::shared_ptr<spu_function_t>> m_db;

	// For internal use
	std::shared_ptr<spu_function_t> find(const be_t<u32>* data, u64 key, u32 max_size);

public:
	SPUDatabase();
	~SPUDatabase();

	// Try to retrieve SPU function information
	std::shared_ptr<spu_function_t> analyse(const be_t<u32>* ls, u32 entry, u32 limit = 0x40000);
};
