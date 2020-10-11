#pragma once
#include <stdbool.h>

#include "riscv_conf.h"
#include "riscv.h"

#include "../tinycg/tinycg.h"
//#include "ir.h"

#define RV_NUM_REGS 32

// csrs
enum {
  // floating point
  CSR_FFLAGS    = 0x001,
  CSR_FRM       = 0x002,
  CSR_FCSR      = 0x003,
  // maching status
  CSR_MSTATUS   = 0x300,
  // low words
  CSR_CYCLE     = 0xb00, // 0xC00,
  CSR_TIME      = 0xC01,
  CSR_INSTRET   = 0xC02,
  // high words
  CSR_CYCLEH    = 0xC80,
  CSR_TIMEH     = 0xC81,
  CSR_INSTRETH  = 0xC82
};

// instruction decode masks
enum {
  //               ....xxxx....xxxx....xxxx....xxxx
  INST_6_2     = 0b00000000000000000000000001111100,
  //               ....xxxx....xxxx....xxxx....xxxx
  FR_OPCODE    = 0b00000000000000000000000001111111, // r-type
  FR_RD        = 0b00000000000000000000111110000000,
  FR_FUNCT3    = 0b00000000000000000111000000000000,
  FR_RS1       = 0b00000000000011111000000000000000,
  FR_RS2       = 0b00000001111100000000000000000000,
  FR_FUNCT7    = 0b11111110000000000000000000000000,
  //               ....xxxx....xxxx....xxxx....xxxx
  FI_IMM_11_0  = 0b11111111111100000000000000000000, // i-type
  //               ....xxxx....xxxx....xxxx....xxxx
  FS_IMM_4_0   = 0b00000000000000000000111110000000, // s-type
  FS_IMM_11_5  = 0b11111110000000000000000000000000,
  //               ....xxxx....xxxx....xxxx....xxxx
  FB_IMM_11    = 0b00000000000000000000000010000000, // b-type
  FB_IMM_4_1   = 0b00000000000000000000111100000000,
  FB_IMM_10_5  = 0b01111110000000000000000000000000,
  FB_IMM_12    = 0b10000000000000000000000000000000,
  //               ....xxxx....xxxx....xxxx....xxxx
  FU_IMM_31_12 = 0b11111111111111111111000000000000, // u-type
  //               ....xxxx....xxxx....xxxx....xxxx
  FJ_IMM_19_12 = 0b00000000000011111111000000000000, // j-type
  FJ_IMM_11    = 0b00000000000100000000000000000000,
  FJ_IMM_10_1  = 0b01111111111000000000000000000000,
  FJ_IMM_20    = 0b10000000000000000000000000000000,
  //               ....xxxx....xxxx....xxxx....xxxx
  FR4_FMT      = 0b00000110000000000000000000000000, // r4-type
  FR4_RS3      = 0b11111000000000000000000000000000,
  //               ....xxxx....xxxx....xxxx....xxxx
};

// a translated basic block
struct block_t {
  // number of instructions encompased
  uint32_t instructions;
  // address range of the basic block
  uint32_t pc_start;
  uint32_t pc_end;
  // next block prediction
  struct block_t *predict;
  // code gen structure
  struct cg_state_t cg;
  // start of this blocks code
  uint8_t code[];
};

struct block_builder_t {
  struct block_t *block;
  // TODO: move cg in here?
};

struct riscv_jit_t {
  // memory range for code buffer
  uint8_t *start;
  uint8_t *end;
  // code buffer write point
  uint8_t *head;
  // block hash map
  uint32_t block_map_size;
  struct block_t **block_map;
};

struct riscv_t {
  // io interface
  struct riscv_io_t io;
  // integer registers
  riscv_word_t X[RV_NUM_REGS];
  riscv_word_t PC;
  // user provided data
  riscv_user_t userdata;
  // exception status
  riscv_exception_t exception;
  // CSRs
  uint64_t csr_cycle;
  uint32_t csr_mstatus;
#if RISCV_VM_SUPPORT_RV32F
  // float registers
  riscv_float_t F[RV_NUM_REGS];
  uint32_t csr_fcsr;
#endif  // RISCV_VM_SUPPORT_RV32F
  // jit specific data
  struct riscv_jit_t jit;
};

// decode rd field
static inline uint32_t dec_rd(uint32_t inst) {
  return (inst & FR_RD) >> 7;
}

// decode rs1 field
static inline uint32_t dec_rs1(uint32_t inst) {
  return (inst & FR_RS1) >> 15;
}

// decode rs2 field
static inline uint32_t dec_rs2(uint32_t inst) {
  return (inst & FR_RS2) >> 20;
}

// decoded funct3 field
static inline uint32_t dec_funct3(uint32_t inst) {
  return (inst & FR_FUNCT3) >> 12;
}

// decode funct7 field
static inline uint32_t dec_funct7(uint32_t inst) {
  return (inst & FR_FUNCT7) >> 25;
}

// decode utype instruction immediate
static inline uint32_t dec_utype_imm(uint32_t inst) {
  return inst & FU_IMM_31_12;
}

// decode jtype instruction immediate
static inline int32_t dec_jtype_imm(uint32_t inst) {
  uint32_t dst = 0;
  dst |= (inst & FJ_IMM_20);
  dst |= (inst & FJ_IMM_19_12) << 11;
  dst |= (inst & FJ_IMM_11)    << 2;
  dst |= (inst & FJ_IMM_10_1)  >> 9;
  // note: shifted to 2nd least significant bit
  return ((int32_t)dst) >> 11;
}

// decode itype instruction immediate
static inline int32_t dec_itype_imm(uint32_t inst) {
  return ((int32_t)(inst & FI_IMM_11_0)) >> 20;
}

// decode r4type format field
static inline uint32_t dec_r4type_fmt(uint32_t inst) {
  return (inst & FR4_FMT) >> 25;
}

// decode r4type rs3 field
static inline uint32_t dec_r4type_rs3(uint32_t inst) {
  return (inst & FR4_RS3) >> 27;
}

// decode csr instruction immediate (same as itype, zero extend)
static inline uint32_t dec_csr(uint32_t inst) {
  return ((uint32_t)(inst & FI_IMM_11_0)) >> 20;
}

// decode btype instruction immediate
static inline int32_t dec_btype_imm(uint32_t inst) {
  uint32_t dst = 0;
  dst |= (inst & FB_IMM_12);
  dst |= (inst & FB_IMM_11) << 23;
  dst |= (inst & FB_IMM_10_5) >> 1;
  dst |= (inst & FB_IMM_4_1) << 12;
  // note: shifted to 2nd least significant bit
  return ((int32_t)dst) >> 19;
}

// decode stype instruction immediate
static inline int32_t dec_stype_imm(uint32_t inst) {
  uint32_t dst = 0;
  dst |= (inst & FS_IMM_11_5);
  dst |= (inst & FS_IMM_4_0) << 13;
  return ((int32_t)dst) >> 20;
}

// sign extend a 16 bit value
static inline uint32_t sign_extend_h(uint32_t x) {
  return (int32_t)((int16_t)x);
}

// sign extend an 8 bit value
static inline uint32_t sign_extend_b(uint32_t x) {
  return (int32_t)((int8_t)x);
}

bool rv_init_jit(struct riscv_t *rv);
bool rv_step_jit(struct riscv_t *rv, const uint64_t cycles_target);