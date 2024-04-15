#ifndef _GROUP_H_
#define _GROUP_H_

typedef struct group *Group;

Group group_init();

void group_setRootId(Group group, char *rootId);
void group_addPoint(Group group, char *pointId);

void group_destroy(Group group);

#endif