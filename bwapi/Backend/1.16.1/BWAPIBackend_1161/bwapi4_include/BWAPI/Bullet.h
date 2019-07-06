#pragma once
#include <BWAPI/Position.h>

namespace BWAPI
{
  // Forward declarations
  class PlayerInterface;
  typedef PlayerInterface *Player;
  
  class BulletType;

  class BulletInterface;
  typedef BulletInterface *Bullet;

  class UnitInterface;
  typedef UnitInterface *Unit;

  /// <summary>An interface object representing a bullet or missile spawned from an attack.</summary>
  ///
  /// The Bullet interface allows you to detect bullets, missiles, and other types
  /// of non-melee attacks or special abilities that would normally be visible through
  /// human eyes (A lurker spike or a Queen's flying parasite), allowing quicker reaction
  /// to unavoidable consequences.
  ///
  /// For example, ordering medics to restore units that are about to receive a lockdown
  /// to compensate for latency and minimize its effects. You can't know entirely which unit
  /// will be receiving a lockdown unless you can detect the lockdown missile using the
  /// Bullet class.
  ///
  /// Bullet objects are re-used after they are destroyed, however their ID is updated when it
  /// represents a new Bullet.
  ///
  /// If Flag::CompleteMapInformation is disabled, then a Bullet is accessible if and only if
  /// it is visible. Otherwise if Flag::CompleteMapInformation is enabled, then all Bullets
  /// in the game are accessible.
  /// @see Game::getBullets, BulletInterface::exists
  /// @ingroup Interface
  class BulletInterface
  {
  protected:
    virtual ~BulletInterface() {};
  public:
    virtual int getID() const = 0;
    
    virtual bool exists() const = 0;

    virtual Player getPlayer() const = 0;

    virtual BulletType getType() const = 0;

    virtual Unit getSource() const = 0;

    virtual Position getPosition() const = 0;

    virtual double getAngle() const = 0;

    virtual double getVelocityX() const = 0;

    virtual double getVelocityY() const = 0;

    virtual Unit getTarget() const = 0;

    virtual Position getTargetPosition() const = 0;

    virtual int getRemoveTimer() const = 0;

    virtual bool isVisible(Player player = nullptr) const = 0;
  };
}