#include "Assembly.h"

#include "BWAPI/GameImpl.h"
#include "BWAPI/PlayerImpl.h"

// PLEASE LOOK AWAY. THIS IS SO BAD AND GROSS AND DISGUSTING AND SHAMEFUL. TURN AWAY NOW WHILE YOU STILL CAN.
// TODO Get rid of this somehow.

unsigned d_eaxSave, d_ebxSave, d_ecxSave, d_edxSave, d_esiSave, d_ediSave, d_espSave, d_ebpSave;

#define SAVE_REGISTERS() __asm {  __asm mov d_eaxSave, eax  \
                                  __asm mov d_ebxSave, ebx  \
                                  __asm mov d_ecxSave, ecx  \
                                  __asm mov d_edxSave, edx  \
                                  __asm mov d_esiSave, esi  \
                                  __asm mov d_ediSave, edi  \
                                  __asm mov d_espSave, esp  \
                                  __asm mov d_ebpSave, ebp }

#define LOAD_REGISTERS() __asm {  __asm mov eax, d_eaxSave  \
                                  __asm mov ebx, d_ebxSave  \
                                  __asm mov ecx, d_ecxSave  \
                                  __asm mov edx, d_edxSave  \
                                  __asm mov esi, d_esiSave  \
                                  __asm mov edi, d_ediSave  \
                                  __asm mov esp, d_espSave  \
                                  __asm mov ebp, d_ebpSave }


#pragma warning( disable: 4740 )

//--------------------------------------------- REPAIR HOOK --------------------------------------------------
void __declspec(naked) _repairHook()
{
  SAVE_REGISTERS();

  BWAPI4::BroodwarImpl.players[d_eaxSave]->_repairedMinerals += d_ebxSave;
  BWAPI4::BroodwarImpl.players[d_eaxSave]->_repairedGas      += d_ediSave;

  LOAD_REGISTERS();
  __asm
  {
    mov ecx, dword ptr ds:[eax*4+0x57f120]
    jmp BW::BWFXN_SpendRepairReturnAddress
  }
}

//--------------------------------------------- REFUND MINERALS HOOK --------------------------------------------
void __declspec(naked) _refundMineralsHook()
{
  SAVE_REGISTERS();
  BWAPI4::BroodwarImpl.players[d_ecxSave/4]->_refundedMinerals += d_eaxSave;

  LOAD_REGISTERS();
  __asm
  {
    movzx eax, word ptr ds:[esi+0x65fd00]
    jmp BW::BWFXN_RefundMineralsReturnAddress
  }
}


//--------------------------------------------- REFUND GAS HOOK --------------------------------------------
void __declspec(naked) _refundGasHook()
{
  SAVE_REGISTERS();
  BWAPI4::BroodwarImpl.players[d_ecxSave/4]->_refundedGas += d_eaxSave;

  LOAD_REGISTERS();
  __asm
  {
    mov dword ptr ds:[ecx+0x57F120], edx
    jmp BW::BWFXN_RefundGasReturnAddress
  }
}


//--------------------------------------------- REFUND MIN AND GAS 2 HOOK --------------------------------------------
void __declspec(naked) _refundMinAndGas2Hook()
{
  SAVE_REGISTERS();
  BWAPI4::BroodwarImpl.players[d_eaxSave/4]->_refundedMinerals += d_edxSave;
  BWAPI4::BroodwarImpl.players[d_eaxSave/4]->_refundedGas += d_ecxSave;

  LOAD_REGISTERS();
  __asm
  {
    add dword ptr ds:[eax+0x57F0F0], edx
    jmp BW::BWFXN_RefundMinAndGas2ReturnAddress
  }
}



//--------------------------------------------- REFUND MIN 3 HOOK --------------------------------------------
void __declspec(naked) _refundMin3Hook()
{
  SAVE_REGISTERS();
  BWAPI4::BroodwarImpl.players[d_ecxSave/4]->_refundedMinerals += d_eaxSave;

  LOAD_REGISTERS();
  __asm
  {
    movzx eax, word ptr ds:[edi+0x65FD00]
    jmp BW::BWFXN_RefundMin3ReturnAddress
  }
}


//--------------------------------------------- REFUND GAS 3 HOOK --------------------------------------------
void __declspec(naked) _refundGas3Hook()
{
  SAVE_REGISTERS();
  BWAPI4::BroodwarImpl.players[d_ecxSave/4]->_refundedGas += d_eaxSave;

  LOAD_REGISTERS();
  __asm
  {
    mov dword ptr ds:[ecx+0x57F120], edx
    jmp BW::BWFXN_RefundGas3ReturnAddress
  }
}


//--------------------------------------------- REFUND MIN 4 HOOK --------------------------------------------
void __declspec(naked) _refundMin4Hook()
{
  SAVE_REGISTERS();
  BWAPI4::BroodwarImpl.players[d_esiSave]->_refundedMinerals += d_edxSave;

  LOAD_REGISTERS();
  __asm
  {
    cmp cx, 0x2e
    mov dword ptr ds:[esi*4+0x57F0F0], edi
    jmp BW::BWFXN_RefundMin4ReturnAddress
  }
}


//--------------------------------------------- REFUND GAS 4 HOOK --------------------------------------------
void __declspec(naked) _refundGas4Hook()
{
  SAVE_REGISTERS();
  BWAPI4::BroodwarImpl.players[d_esiSave]->_refundedGas += d_edxSave;

  LOAD_REGISTERS();
  __asm
  {
    lea ecx, dword ptr ds:[esi*4+0x57F120]
    jmp BW::BWFXN_RefundGas4ReturnAddress
  }
}

//--------------------------------------------- REFUND MIN AND GAS 5 HOOK --------------------------------------------
void __declspec(naked) _refundMinAndGas5Hook()
{
  SAVE_REGISTERS();
  BWAPI4::BroodwarImpl.players[d_eaxSave/4]->_refundedMinerals += d_esiSave;
  BWAPI4::BroodwarImpl.players[d_eaxSave/4]->_refundedGas += d_ecxSave;

  LOAD_REGISTERS();
  __asm
  {
    add dword ptr ds:[eax+0x57F120], ecx
    jmp BW::BWFXN_RefundMinAndGas5ReturnAddress
  }
}

#pragma warning( default: 4740 )
