#ifndef _GROUP_H_
#define _GROUP_H_

typedef struct group *Group;

Group group_init();

void group_setRootId(Group group, char *rootId);
void group_addPoint(Group group, char *pointId);

char *group_getRootId(Group group);
int group_getSize(Group group);
char *group_getPointId(Group group, int index);

void group_destroy(Group group);

#endif