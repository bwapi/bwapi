#include "Assembly.h"

#include <windows.h>

#include "BWAPI/GameImpl.h"
#include "BWAPI/PlayerImpl.h"
#include "BW/Offsets.h"

DWORD d_eaxSave, d_ebxSave, d_ecxSave, d_edxSave, d_esiSave, d_ediSave, d_espSave, d_ebpSave;

//--------------------------------------------- REPAIR HOOK --------------------------------------------------
void __declspec(naked) _repairHook()
{
  
  __asm
  {
    mov d_eaxSave, eax
    mov d_ebxSave, ebx
    mov d_ecxSave, ecx
    mov d_edxSave, edx
    mov d_esiSave, esi
    mov d_ediSave, edi
    mov d_espSave, esp
    mov d_ebpSave, ebp
  }
  BWAPI::BroodwarImpl.players[d_eaxSave]->_repairedMinerals += d_ebxSave;
  BWAPI::BroodwarImpl.players[d_eaxSave]->_repairedGas      += d_ediSave;

  __asm
  {
    mov eax, d_eaxSave
    mov ebx, d_ebxSave
    mov ecx, d_ecxSave
    mov edx, d_edxSave
    mov esi, d_esiSave
    mov edi, d_ediSave
    mov esp, d_espSave
    mov ebp, d_ebpSave

    mov ecx, dword ptr ds:[eax*4+0x57f120]
    jmp BW::BWFXN_SpendRepairReturnAddress
  }
}

//--------------------------------------------- REFUND MINERALS HOOK --------------------------------------------
void __declspec(naked) _refundMineralsHook()
{
  
  __asm
  {
    mov d_eaxSave, eax
    mov d_ebxSave, ebx
    mov d_ecxSave, ecx
    mov d_edxSave, edx
    mov d_esiSave, esi
    mov d_ediSave, edi
    mov d_espSave, esp
    mov d_ebpSave, ebp
  }
  BWAPI::BroodwarImpl.players[d_ecxSave/4]->_refundedMinerals += d_eaxSave;

  __asm
  {
    mov eax, d_eaxSave
    mov ebx, d_ebxSave
    mov ecx, d_ecxSave
    mov edx, d_edxSave
    mov esi, d_esiSave
    mov edi, d_ediSave
    mov esp, d_espSave
    mov ebp, d_ebpSave

    movzx eax, word ptr ds:[esi+0x65fd00]
    jmp BW::BWFXN_RefundMineralsReturnAddress
  }
}


//--------------------------------------------- REFUND GAS HOOK --------------------------------------------
void __declspec(naked) _refundGasHook()
{
  
  __asm
  {
    mov d_eaxSave, eax
    mov d_ebxSave, ebx
    mov d_ecxSave, ecx
    mov d_edxSave, edx
    mov d_esiSave, esi
    mov d_ediSave, edi
    mov d_espSave, esp
    mov d_ebpSave, ebp
  }
  BWAPI::BroodwarImpl.players[d_ecxSave/4]->_refundedGas += d_eaxSave;

  __asm
  {
    mov eax, d_eaxSave
    mov ebx, d_ebxSave
    mov ecx, d_ecxSave
    mov edx, d_edxSave
    mov esi, d_esiSave
    mov edi, d_ediSave
    mov esp, d_espSave
    mov ebp, d_ebpSave

    mov dword ptr ds:[ecx+0x57F120], edx
    jmp BW::BWFXN_RefundGasReturnAddress
  }
}


//--------------------------------------------- REFUND MIN AND GAS 2 HOOK --------------------------------------------
void __declspec(naked) _refundMinAndGas2Hook()
{
  
  __asm
  {
    mov d_eaxSave, eax
    mov d_ebxSave, ebx
    mov d_ecxSave, ecx
    mov d_edxSave, edx
    mov d_esiSave, esi
    mov d_ediSave, edi
    mov d_espSave, esp
    mov d_ebpSave, ebp
  }
  BWAPI::BroodwarImpl.players[d_eaxSave/4]->_refundedMinerals += d_edxSave;
  BWAPI::BroodwarImpl.players[d_eaxSave/4]->_refundedGas += d_ecxSave;

  __asm
  {
    mov eax, d_eaxSave
    mov ebx, d_ebxSave
    mov ecx, d_ecxSave
    mov edx, d_edxSave
    mov esi, d_esiSave
    mov edi, d_ediSave
    mov esp, d_espSave
    mov ebp, d_ebpSave

    add dword ptr ds:[eax+0x57F0F0], edx
    jmp BW::BWFXN_RefundMinAndGas2ReturnAddress
  }
}



//--------------------------------------------- REFUND MIN 3 HOOK --------------------------------------------
void __declspec(naked) _refundMin3Hook()
{
  
  __asm
  {
    mov d_eaxSave, eax
    mov d_ebxSave, ebx
    mov d_ecxSave, ecx
    mov d_edxSave, edx
    mov d_esiSave, esi
    mov d_ediSave, edi
    mov d_espSave, esp
    mov d_ebpSave, ebp
  }
  BWAPI::BroodwarImpl.players[d_ecxSave/4]->_refundedMinerals += d_eaxSave;

  __asm
  {
    mov eax, d_eaxSave
    mov ebx, d_ebxSave
    mov ecx, d_ecxSave
    mov edx, d_edxSave
    mov esi, d_esiSave
    mov edi, d_ediSave
    mov esp, d_espSave
    mov ebp, d_ebpSave

    movzx eax, word ptr ds:[edi+0x65FD00]
    jmp BW::BWFXN_RefundMin3ReturnAddress
  }
}


//--------------------------------------------- REFUND GAS 3 HOOK --------------------------------------------
void __declspec(naked) _refundGas3Hook()
{
  
  __asm
  {
    mov d_eaxSave, eax
    mov d_ebxSave, ebx
    mov d_ecxSave, ecx
    mov d_edxSave, edx
    mov d_esiSave, esi
    mov d_ediSave, edi
    mov d_espSave, esp
    mov d_ebpSave, ebp
  }
  BWAPI::BroodwarImpl.players[d_ecxSave/4]->_refundedGas += d_eaxSave;

  __asm
  {
    mov eax, d_eaxSave
    mov ebx, d_ebxSave
    mov ecx, d_ecxSave
    mov edx, d_edxSave
    mov esi, d_esiSave
    mov edi, d_ediSave
    mov esp, d_espSave
    mov ebp, d_ebpSave

    mov dword ptr ds:[ecx+0x57F120], edx
    jmp BW::BWFXN_RefundGas3ReturnAddress
  }
}


//--------------------------------------------- REFUND MIN 4 HOOK --------------------------------------------
void __declspec(naked) _refundMin4Hook()
{
  
  __asm
  {
    mov d_eaxSave, eax
    mov d_ebxSave, ebx
    mov d_ecxSave, ecx
    mov d_edxSave, edx
    mov d_esiSave, esi
    mov d_ediSave, edi
    mov d_espSave, esp
    mov d_ebpSave, ebp
  }
  BWAPI::BroodwarImpl.players[d_esiSave]->_refundedMinerals += d_edxSave;

  __asm
  {
    mov eax, d_eaxSave
    mov ebx, d_ebxSave
    mov ecx, d_ecxSave
    mov edx, d_edxSave
    mov esi, d_esiSave
    mov edi, d_ediSave
    mov esp, d_espSave
    mov ebp, d_ebpSave
    cmp cx, 0x2e
    mov dword ptr ds:[esi*4+0x57F0F0], edi
    jmp BW::BWFXN_RefundMin4ReturnAddress
  }
}


//--------------------------------------------- REFUND GAS 4 HOOK --------------------------------------------
void __declspec(naked) _refundGas4Hook()
{
  
  __asm
  {
    mov d_eaxSave, eax
    mov d_ebxSave, ebx
    mov d_ecxSave, ecx
    mov d_edxSave, edx
    mov d_esiSave, esi
    mov d_ediSave, edi
    mov d_espSave, esp
    mov d_ebpSave, ebp
  }
  BWAPI::BroodwarImpl.players[d_esiSave]->_refundedGas += d_edxSave;

  __asm
  {
    mov eax, d_eaxSave
    mov ebx, d_ebxSave
    mov ecx, d_ecxSave
    mov edx, d_edxSave
    mov esi, d_esiSave
    mov edi, d_ediSave
    mov esp, d_espSave
    mov ebp, d_ebpSave

    lea ecx, dword ptr ds:[esi*4+0x57F120]
    jmp BW::BWFXN_RefundGas4ReturnAddress
  }
}

//--------------------------------------------- REFUND MIN AND GAS 5 HOOK --------------------------------------------
void __declspec(naked) _refundMinAndGas5Hook()
{
  
  __asm
  {
    mov d_eaxSave, eax
    mov d_ebxSave, ebx
    mov d_ecxSave, ecx
    mov d_edxSave, edx
    mov d_esiSave, esi
    mov d_ediSave, edi
    mov d_espSave, esp
    mov d_ebpSave, ebp
  }
  BWAPI::BroodwarImpl.players[d_eaxSave/4]->_refundedMinerals += d_esiSave;
  BWAPI::BroodwarImpl.players[d_eaxSave/4]->_refundedGas += d_ecxSave;

  __asm
  {
    mov eax, d_eaxSave
    mov ebx, d_ebxSave
    mov ecx, d_ecxSave
    mov edx, d_edxSave
    mov esi, d_esiSave
    mov edi, d_ediSave
    mov esp, d_espSave
    mov ebp, d_ebpSave

    add dword ptr ds:[eax+0x57F120], ecx
    jmp BW::BWFXN_RefundMinAndGas5ReturnAddress
  }
}
