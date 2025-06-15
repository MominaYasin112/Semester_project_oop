#ifndef SKILL_REPOSITORY_H
#define SKILL_REPOSITORY_H

#include "Skill.h"
#include "Dynamic_array.h"
#include "AppException.h"

class SkillRepository {
private:
    Dynamic_array<Skill> availableSkills;

public:
    SkillRepository();

    const Skill& findSkillByName(const Str& name) const;
    Dynamic_array<Str> getAllSkillNames() const;
    const Dynamic_array<Skill>& getAllSkills() const;
};

#endif
