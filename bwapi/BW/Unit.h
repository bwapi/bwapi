/** 
 * Interface of broodwar unit, can be used to obain any information and issue commands.
 */
#include "Types.h"
struct BW_Unit;
class UnitPrototype;
class AbilityPrototype;
class Player;

class Unit
{
  public :
    /** Constructor
     * @param rawData pointer to broodwar unit data structure, the class will 
     *                use that object to load information over time.
     */
    Unit(BW_Unit *rawData);
    ~Unit();
    /**
     * Gets health points of the current unit, note that this value can be lower 
     * (by one) than the displayed value in in broodwar as broodwar doesn't want 
     * to confuse users with 0 hitpoint unit (with non-zero hp fraction).
     * @returns Integral part of hitpoints.
     */
    u16 getHealthPoints() const; 
    /**
     * Gets the fraction of hitpoints 
     * @returns values in interval 0-255 where 256 is equal to one hitpoint.
     */
    u8 getHealthPointsFraction() const;
    Player* getOwner() const;
    const UnitPrototype* const getPrototype() const;
    bool canOrder(const AbilityPrototype* const ability, Unit* target) const;
    bool canOrder(const AbilityPrototype* const ability, BW_Position target) const;
    void order(const AbilityPrototype* const ability, Unit* target);
    void order(const AbilityPrototype* const ability, BW_Position target);
  private :
    BW_Unit* rawData; /**< Pointer to broodwar unit data */
    UnitPrototype* prototype;
    Player* owner;
};