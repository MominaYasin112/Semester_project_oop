#include "Skill.h"
#include<iostream>
#include<fstream>
using namespace std;
#include "AppException.h"
Skill::Skill() : skillId(0), name(""), category(""), description("") {}

Skill::Skill(int id, Str name, Str category, Str description)
    : skillId(id), name(name), category(category), description(description) {
}

Skill::Skill(const Skill& other)
    : skillId(other.skillId), name(other.name), category(other.category), description(other.description) {
}

Skill& Skill::operator=(const Skill& other) {
    if (this != &other) {
        skillId = other.skillId;
        name = other.name;
        category = other.category;
        description = other.description;
    }
    return *this;
}

Skill::~Skill() {}


void Skill::setName(const Str& name_) {
    if (name_.length(name_.return_array()) == 0)
        throw SkillValidationException("Skill name cannot be empty.");
    name = name_;
}

void Skill::setCategory(const Str& category_) {
    if (category_.length(category_.return_array()) == 0)
        throw SkillValidationException("Skill category cannot be empty.");
    category = category_;
}

void Skill::setDescription(const Str& desc) {
    if (desc.length(desc.return_array()) < 10)
        throw SkillValidationException("Skill description must be at least 10 characters long.");
    description = desc;
}

void Skill::setSkillId(int id) {
    if (id <= 0)
        throw SkillValidationException("Skill ID must be a positive integer.");
    skillId = id;
}
int Skill::getSkillId() const {
    return skillId;
}

Str Skill::getName() const {
    return name;
}

Str Skill::getCategory() const {
    return category;
}

Str Skill::getDescription() const {
    return description;
}
void Skill::saveToBinary(ofstream& out) const {
    out.write((char*)&skillId, sizeof(skillId));
    name.writeToBinary(out);
    category.writeToBinary(out);
    description.writeToBinary(out);
}

void Skill::loadFromBinary(ifstream& in) {
    in.read((char*)&skillId, sizeof(skillId));
    name.readFromBinary(in);
    category.readFromBinary(in);
    description.readFromBinary(in);
}
