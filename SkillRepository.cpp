#include "SkillRepository.h"

SkillRepository::SkillRepository() {
    availableSkills.push(Skill(1, "Web Development", "Technology", "Build websites using HTML/CSS/JS"));
    availableSkills.push(Skill(2, "Graphic Design", "Design", "Create designs using Photoshop"));
    availableSkills.push(Skill(3, "Public Speaking", "Communication", "Improve presentation skills"));
    availableSkills.push(Skill(4, "Data Analysis", "Data", "Analyze data using Excel or Python"));
    availableSkills.push(Skill(5, "Creative Writing", "Literature", "Write poems, stories or blogs"));
    availableSkills.push(Skill(6, "Photography", "Art", "Capture beautiful moments"));
    availableSkills.push(Skill(7, "Cooking", "Life Skills", "Learn to cook different cuisines"));
    availableSkills.push(Skill(8, "Mobile App Development", "Technology", "Build Android/iOS apps"));
    availableSkills.push(Skill(9, "Game Development", "Technology", "Create games using Unity"));
    availableSkills.push(Skill(10, "Mathematics Tutoring", "Education", "Help with school or college math"));
}

const Skill& SkillRepository::findSkillByName(const Str& name) const {
    for (int i = 0; i < availableSkills.size(); ++i) {
        if (availableSkills[i].getName() == name) {
            return availableSkills[i];
        }
    }
    throw SkillValidationException(Str("Skill '").concat(name).concat("' not found in repository."));
}

const Dynamic_array<Skill>& SkillRepository::getAllSkills() const {
    return availableSkills;
}
Dynamic_array<Str> SkillRepository::getAllSkillNames() const {
    Dynamic_array<Str> names;
    for (int i = 0; i < availableSkills.size(); ++i) {
        names.push(availableSkills[i].getName());
    }
    return names;
}

