//===-- LiveStackAnalysis.h - Live Stack Slot Analysis ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the live stack slot analysis pass. It is analogous to
// live interval analysis except it's analyzing liveness of stack slots rather
// than registers.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_LIVESTACK_ANALYSIS_H
#define LLVM_CODEGEN_LIVESTACK_ANALYSIS_H

#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/LiveInterval.h"
#include "llvm/Target/TargetRegisterInfo.h"
#include "llvm/Support/Allocator.h"
#include <map>

namespace llvm {

  class LiveStacks : public MachineFunctionPass {
    /// Special pool allocator for VNInfo's (LiveInterval val#).
    ///
    BumpPtrAllocator VNInfoAllocator;

    /// S2IMap - Stack slot indices to live interval mapping.
    ///
    typedef std::map<int, LiveInterval> SS2IntervalMap;
    SS2IntervalMap S2IMap;

    /// S2RCMap - Stack slot indices to register class mapping.
    std::map<int, const TargetRegisterClass*> S2RCMap;
    
  public:
    static char ID; // Pass identification, replacement for typeid
    LiveStacks() : MachineFunctionPass(&ID) {}

    typedef SS2IntervalMap::iterator iterator;
    typedef SS2IntervalMap::const_iterator const_iterator;
    const_iterator begin() const { return S2IMap.begin(); }
    const_iterator end() const { return S2IMap.end(); }
    iterator begin() { return S2IMap.begin(); }
    iterator end() { return S2IMap.end(); }

    void scaleNumbering(int factor);

    unsigned getNumIntervals() const { return (unsigned)S2IMap.size(); }

    LiveInterval &getOrCreateInterval(int Slot, const TargetRegisterClass *RC) {
      assert(Slot >= 0 && "Spill slot indice must be >= 0");
      SS2IntervalMap::iterator I = S2IMap.find(Slot);
      if (I == S2IMap.end()) {
        I = S2IMap.insert(I,std::make_pair(Slot, LiveInterval(Slot,0.0F,true)));
        S2RCMap.insert(std::make_pair(Slot, RC));
      } else {
        // Use the largest common subclass register class.
        const TargetRegisterClass *OldRC = S2RCMap[Slot];
        S2RCMap[Slot] = getCommonSubClass(OldRC, RC);
      }
      return I->second;
    }

    LiveInterval &getInterval(int Slot) {
      assert(Slot >= 0 && "Spill slot indice must be >= 0");
      SS2IntervalMap::iterator I = S2IMap.find(Slot);
      assert(I != S2IMap.end() && "Interval does not exist for stack slot");
      return I->second;
    }

    const LiveInterval &getInterval(int Slot) const {
      assert(Slot >= 0 && "Spill slot indice must be >= 0");
      SS2IntervalMap::const_iterator I = S2IMap.find(Slot);
      assert(I != S2IMap.end() && "Interval does not exist for stack slot");
      return I->second;
    }

    bool hasInterval(int Slot) const {
      return S2IMap.count(Slot);
    }

    const TargetRegisterClass *getIntervalRegClass(int Slot) const {
      assert(Slot >= 0 && "Spill slot indice must be >= 0");
      std::map<int, const TargetRegisterClass*>::const_iterator
        I = S2RCMap.find(Slot);
      assert(I != S2RCMap.end() &&
             "Register class info does not exist for stack slot");
      return I->second;
    }

    BumpPtrAllocator& getVNInfoAllocator() { return VNInfoAllocator; }

    virtual void getAnalysisUsage(AnalysisUsage &AU) const;
    virtual void releaseMemory();

    /// runOnMachineFunction - pass entry point
    virtual bool runOnMachineFunction(MachineFunction&);

    /// print - Implement the dump method.
    virtual void print(raw_ostream &O, const Module* = 0) const;
  };
}

#endif /* LLVM_CODEGEN_LIVESTACK_ANALYSIS_H */
