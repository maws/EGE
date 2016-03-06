#pragma once

const unsigned ENTITY_INDEX_BITS = 24;
const unsigned ENTITY_GENERATION_BITS = 8;
const unsigned ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;
const unsigned ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;
const unsigned MIN_NUM_FREE_ENTITIES = 1024;

struct Entity
{
	Entity() {}
	Entity(unsigned id) :m_id(id) {}

	unsigned m_id;
	unsigned getIndex(){ return m_id & ENTITY_INDEX_MASK; }
	unsigned getGeneration() { return (m_id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
};