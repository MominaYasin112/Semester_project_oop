#ifndef SKILL_H
#define SKILL_H

#include "Str.h"

class Skill {
protected:
    int skillId;
    Str name;
    Str category;
    Str description;

public:
    Skill();
    Skill(int id, Str name, Str category, Str description);
    Skill(const Skill& other);
    Skill& operator=(const Skill& other);
    virtual ~Skill();

    // Getters
    int getSkillId() const;
    Str getName() const;
    Str getCategory() const;
    Str getDescription() const;

    // Setters
    void setSkillId(int id);
    void setName(const Str& name);
    void setCategory(const Str& category);
    void setDescription(const Str& description);
    virtual void saveToBinary(ofstream& out) const;
    virtual void loadFromBinary(ifstream& in);
};

#endif
