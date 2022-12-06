#ifndef UGID_FUNCTIONS_H
#define UGID_FUNCTIONS_H
#include <sys/types.h>

char *user_name_from_id(uid_t uid);
uid_t user_id_from_name(const char *name);
char *group_name_from_id(gid_t gid);
gid_t group_id_from_name(const char *name);

#endif
