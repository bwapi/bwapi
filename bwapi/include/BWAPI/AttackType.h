#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class AttackType
  {
    public:
      AttackType();
      AttackType(int id);
      AttackType(const AttackType& other);
      AttackType& operator=(const AttackType& other);
      bool operator==(const AttackType& other) const;
      bool operator!=(const AttackType& other) const;
      bool operator<(const AttackType& other) const;
      int getID() const;
      std::string getName() const;
    private:
      int id;
  };
  namespace AttackTypes
  {
    AttackType getAttackType(std::string& name);
    std::set<AttackType>& allAttackTypes();
    void init();
    extern const AttackType None;
    extern const AttackType Unknown;
  };
}
