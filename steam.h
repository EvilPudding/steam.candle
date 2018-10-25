#ifndef STEAM_H
#define STEAM_H

#include <ecs/ecm.h>

typedef struct c_steam_t
{
	c_t super;
	intptr_t client;
	const char *persona_name;
} c_steam_t;

DEF_CASTER("steam", c_steam, c_steam_t)

c_steam_t *c_steam_new(void);

void *c_steam_sm(c_steam_t *self);

#endif /* !STEAM_H */
