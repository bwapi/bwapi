#pragma once
#include <BWAPI.h>

namespace BWAPI
{
  // Forward declarations
  class PlayerInterface;
  typedef PlayerInterface *Player;
  class BulletType;

  class BulletInterface;
  typedef BulletInterface *Bullet;

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
  class BulletInterface : public Interface<BulletInterface>
  {
  protected:
    virtual ~BulletInterface() {};
  public:
    /// <summary>Retrieves a unique identifier for the current Bullet.</summary>
    ///
    /// @returns
    ///   An integer value containing the identifier.
    virtual int getID() const = 0;
    
    /// <summary>Checks if the Bullet exists in the view of the BWAPI player.</summary>
    ///
    /// @retval true If the bullet exists or is visible.
    /// @retval false If the bullet was destroyed or has gone out of scope.
    ///
    /// If Flag::CompleteMapInformation is disabled, and a Bullet is not visible, then the
    /// return value will be false regardless of the Bullet's true existence. This is because
    /// absolutely no state information on invisible enemy bullets is made available to the AI.
    ///
    /// If Flag::CompleteMapInformation is enabled, then this function is accurate for all
    /// Bullet information.
    /// @see isVisible, UnitInterface::exists
    virtual bool exists() const = 0;

    /// <summary>Retrieves the Player interface that owns the Bullet.</summary>
    ///
    /// @retval nullptr If the Player object for this Bullet is inaccessible.
    ///
    /// @returns
    ///   The owning Player interface object.
    virtual Player getPlayer() const = 0;

    /// <summary>Retrieves the type of this Bullet.</summary>
    ///
    /// @retval BulletTypes::Unknown if the Bullet is inaccessible.
    ///
    /// @returns
    ///   A BulletType representing the Bullet's type.
    virtual BulletType getType() const = 0;

    /// <summary>Retrieves the Unit interface that the Bullet spawned from.</summary>
    ///
    /// @retval nullptr If the source can not be identified or is inaccessible.
    ///
    /// @returns
    ///   The owning Unit interface object.
    /// @see getTarget
    virtual Unit getSource() const = 0;

    /// <summary>Retrieves the Bullet's current position.</summary>
    ///
    /// @retval Positions::Unknown If the Bullet is inaccessible.
    ///
    /// @returns
    ///   A Position containing the Bullet's current coordinates.
    /// @see getTargetPosition
    virtual Position getPosition() const = 0;

    /// <summary>Retrieve's the direction the Bullet is facing.</summary> If the angle is 0, then
    /// the Bullet is facing right.
    /// 
    /// @retval 0.0 If the bullet is inaccessible.
    ///
    /// @returns
    ///   A double representing the direction the Bullet is facing.
    virtual double getAngle() const = 0;

    /// <summary>Retrieves the X component of the Bullet's velocity, measured in pixels per frame.</summary>
    ///
    /// @retval 0.0 if the Bullet is inaccessible.
    ///
    /// @returns
    ///   A double representing the number of pixels moved on the X axis per frame.
    ///
    /// @see getVelocityY, getAngle
    virtual double getVelocityX() const = 0;

    /// <summary>Retrieves the Y component of the Bullet's velocity, measured in pixels per frame.</summary>
    ///
    /// @retval 0.0 if the Bullet is inaccessible.
    ///
    /// @returns
    ///   A double representing the number of pixels moved on the Y axis per frame.
    ///
    /// @see getVelocityX, getAngle
    virtual double getVelocityY() const = 0;

    /// <summary>Retrieves the Unit interface that the Bullet is heading to.</summary>
    ///
    /// @retval nullptr If the Bullet's target Unit is inaccessible, the Bullet is targetting the
    /// ground, or if the Bullet itself is inaccessible.
    ///
    /// @returns
    ///   The target Unit interface object, if one exists.
    /// @see getTargetPosition, getSource
    virtual Unit getTarget() const = 0;

    /// <summary>Retrieves the target position that the Bullet is heading to.</summary>
    ///
    /// @retval Positions::Unknown If the bullet is inaccessible.
    ///
    /// @returns
    ///   A Position indicating where the Bullet is headed.
    /// @see getTarget, getPosition
    virtual Position getTargetPosition() const = 0;

    /// <summary>Retrieves the timer that indicates the Bullet's life span.</summary>
    ///
    /// Bullets are not permanent objects, so they will often have a limited life span.
    /// This life span is measured in frames. Normally a Bullet will reach its target
    /// before being removed.
    ///
    /// @retval 0 If the Bullet is inaccessible.
    ///
    /// @returns
    ///   An integer representing the remaining number of frames until the Bullet self-destructs.
    virtual int getRemoveTimer() const = 0;

    /// <summary>Retrieves the visibility state of the Bullet.</summary>
    ///
    /// <param name="player"> (optional)
    ///   If this parameter is specified, then the Bullet's visibility to the given player is
    ///   checked. If this parameter is omitted, then a default value of nullptr is used, which
    ///   will check if the BWAPI player has vision of the Bullet.
    /// </param>
    ///
    /// @note If \c player is nullptr and Broodwar->self() is also nullptr, then the visibility of
    /// the Bullet is determined by checking if at least one other player has vision of the
    /// Bullet.
    ///
    /// @retval true If the Bullet is visible to the specified player.
    /// @retval false If the Bullet is not visible to the specified player.
    virtual bool isVisible(Player player = nullptr) const = 0;
  };
}