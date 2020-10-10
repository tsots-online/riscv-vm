#include <assert.h>
#include <string.h>

#include "ir.h"


enum {
  op_imm,
  op_ld_reg,
  op_st_reg,
  op_st_pc,
  op_add,
  op_sub,
  op_and,
  op_or,
  op_xor,
  op_sltu,
  op_slt,
  op_shl,
  op_sal,
  op_sll,
  op_mul,
  op_imul,
};

struct ir_inst_t *ir_alloc(struct ir_block_t *block) {
  struct ir_inst_t *i = block->inst + (block->head++);
  assert(block->head < IR_MAX_INST);
  memset(i, 0, sizeof(struct ir_inst_t));
  return i;
}

void ir_init(struct ir_block_t *block) {
  memset(block, 0, sizeof(struct ir_block_t));
}

struct ir_inst_t *ir_imm(struct ir_block_t *block, int32_t imm) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_imm;
  i->imm = imm;
  return i;
}

struct ir_inst_t *ir_ld_reg(struct ir_block_t *block, int32_t offset) {
  struct ir_inst_t *i = block->inst + (block->head++);
  i->op = op_ld_reg;
  i->offset = offset;
  return i;
}

struct ir_inst_t *ir_st_reg(struct ir_block_t *block, int32_t offset, struct ir_inst_t *val) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_st_reg;
  i->offset = offset;
  i->value = val;
  val->parent = i;
  return i;
}

struct ir_inst_t *ir_add(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_add;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_sub(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_sub;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_and(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_and;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_or(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_or;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_xor(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_xor;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_sltu(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_sltu;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_slt(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_slt;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_shl(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_shl;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_sal(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_sal;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_sll(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_sll;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_mul(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_mul;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}

struct ir_inst_t *ir_imul(struct ir_block_t *block, struct ir_inst_t *lhs, struct ir_inst_t *rhs) {
  struct ir_inst_t *i = ir_alloc(block);
  i->op = op_imul;
  i->lhs = lhs;
  i->rhs = rhs;
  lhs->parent = i;
  rhs->parent = i;
  return i;
}