#pragma once
#include <BWAPI/Position.h>

namespace BWAPI
{
  class UnitInterface;
  typedef UnitInterface *Unit;

  /// <summary>PositionOrUnit is a class that is either a Position <b>or</b> a Unit.</summary>
  /// The purpose of this class is to reduce interface overhead by combining two commonly passed
  /// values into one, and to allow more function overloads with less effort.
  class PositionOrUnit
  {
  public:
    /// <summary>Unit constructor. Assigns this class to contain a unit.</summary>
    ///
    /// <param name="unit">
    ///     The unit to assign to this class.
    /// </param>
    PositionOrUnit(Unit unit = nullptr);

    /// <summary>Position constructor. Assigns this class to contain a position.</summary>
    ///
    /// <param name="pos">
    ///     The position to assign to this class.
    /// </param>
    PositionOrUnit(Position pos);

    /// <summary>Unit assignment operator. Assigns a unit to this class.</summary>
    PositionOrUnit &operator =(Unit pUnit);

    /// <summary>Position assignment operator. Assigns a position to this class.</summary>
    PositionOrUnit &operator =(Position pos);

    /// <summary>Indicates if a Unit is currently held in this class.</summary>
    ///
    /// @returns true if the value contained within this class is considered a unit, and false if
    /// it is a position.
    bool isUnit() const;

    /// <summary>Retrieves the Unit attached to this class, if there is one.</summary>
    ///
    /// @returns The Unit that was assigned to this class.
    /// @retval nullptr If this class does not contain a unit, or if nullptr was assigned to
    /// this class as a Unit.
    Unit getUnit() const;

    /// <summary>Indicates if a Position is currently held in this class.</summary>
    ///
    /// @returns true if the value contained within this class is considered a position, and false
    /// if it is a unit.
    bool isPosition() const;

    /// <summary>Retrieves the position if it was assigned to this class, otherwise it will
    /// retrieve the position of the unit contained within this class if there is one.</summary>
    ///
    /// @returns Position that was stored if there is one. If not, then the position of the unit
    /// will be used instead.
    /// @retval Positions::None if a nullptr Unit was assigned to this class.
    Position getPosition() const;

  private:
    /// @cond HIDDEN
    union
    {
      struct
      {
        int x, y;
      } position;
      struct
      {
        Unit pUnit;
        int y;
      } unit;
    };
    /// @endcond
  };

}
