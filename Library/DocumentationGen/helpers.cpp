#include "helpers.h"

#include <string>

unsigned iconWeapon[WeaponTypes::Enum::MAX] = { 323, 323, 324, 324, 325, 325, 364, 326, 327, 326, 327, 328, 328, 329, 330, 331, 332, 331, 332, 333, 334, 333, 334, 333, 334, 335, 335, 336, 336, 337, 251, 311, 240, 241, 241, 338, 338, 338, 339, 339, 340, 340, 341, 342, 342, 343, 344, 344, 345, 345, 346, 346, 347, 348, 349, 350, 271, 40, 267, 270, 265, 265, 351, 352, 353, 353, 354, 354, 355, 355, 356, 356, 357, 358, 359, 358, 359, 360, 360, 361, 362, 362, 85, 278, 278, 353, 353, 363, 0, 0, 0, 0, 0, 0, 0, 0, 326, 327, 335, 327, 332, 359, 241, 331, 344, 332, 332, 332, 240, 382, 323, 353, 324, 324, 358, 359, 324, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323 };
unsigned iconUpgrade[UpgradeTypes::Enum::MAX] = { 292, 293, 291, 297, 298, 303, 304, 288, 289, 290, 299, 300, 301, 305, 306, 310, 238, 287, 239, 248, 249, 256, 284, 285, 261, 262, 296, 263, 264, 266, 268, 294, 295, 281, 307, 314, 315, 316, 317, 276, 318, 319, 320, 321, 322, 0, 0, 383, 0, 385, 0, 384, 379, 378, 380, 0, 0, 0, 0, 0, 0 };
unsigned iconTech[TechTypes::Enum::MAX] = { 237, 240, 241, 243, 250, 245, 247, 242, 251, 252, 252, 259, 313, 40, 270, 265, 302, 267, 271, 275, 277, 280, 278, 68, 366, 369, 0, 371, 63, 372, 373, 381, 376, 0, 365, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned iconOrder[Orders::Enum::MAX] = { 65535, 229, 229, 229, 65535, 229, 228, 229, 230, 230, 230, 230, 230, 230, 230, 65535, 229, 229, 229, 230, 65535, 65535, 230, 229, 229, 65535, 65535, 313, 313, 313, 65535, 65535, 65535, 232, 232, 232, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 228, 65535, 228, 229, 228, 229, 230, 230, 230, 230, 255, 229, 230, 230, 230, 255, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 283, 282, 65535, 65535, 65535, 65535, 65535, 65535, 231, 231, 231, 231, 231, 233, 231, 231, 231, 231, 231, 233, 231, 228, 229, 309, 229, 228, 65535, 245, 246, 230, 65535, 65535, 65535, 65535, 67, 65535, 255, 255, 252, 253, 312, 312, 251, 251, 240, 259, 65535, 260, 65535, 271, 40, 241, 65535, 65535, 65535, 311, 311, 311, 311, 65535, 65535, 65535, 228, 230, 230, 255, 65535, 65535, 65535, 65535, 247, 65535, 65535, 65535, 65535, 65535, 65535, 277, 65535, 65535, 65535, 254, 65535, 65535, 65535, 65535, 230, 230, 230, 229, 65535, 65535, 65535, 65535, 65535, 229, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 229, 365, 228, 255, 65535, 277, 369, 371, 67, 372, 373, 240, 65535, 65535};

#define NAMESPACE_T(x) std::string const & namespaceOf< x >::value = #x "s";
NAMESPACE_T(BulletType)
NAMESPACE_T(Color)
NAMESPACE_T(DamageType)
NAMESPACE_T(Error)
NAMESPACE_T(ExplosionType)
NAMESPACE_T(GameType)
NAMESPACE_T(Order)
NAMESPACE_T(PlayerType)
NAMESPACE_T(Race)
NAMESPACE_T(TechType)
NAMESPACE_T(UnitCommandType)
NAMESPACE_T(UnitSizeType)
NAMESPACE_T(UnitType)
NAMESPACE_T(UpgradeType)
NAMESPACE_T(WeaponType)
#undef NAMESPACE_T

template <>
std::string tref(std::string const &t)
{
  return t;
}

std::string sprite(std::string const & group, std::string const & name)
{
  return "@m_span{" + group + " " + group + "-" + name + "}@m_endspan\n";
}

std::string tooltipIcon(std::string const & name)
{
  return sprite("tticon", name);
}

std::string cmdButton(std::string const & name)
{
  return sprite("cmdbtn", name);
}

std::string imgOre()
{
  return tooltipIcon("ore");
}

std::string imgEnergy()
{
  return tooltipIcon("energy");
}

std::string imgGas(Race race)
{
  return tooltipIcon("gas" + std::to_string(race.getID() % 3));
}

std::string imgSupply(Race race)
{
  return tooltipIcon("supply" + std::to_string(race.getID() % 3));
}

std::string iconId(unsigned id)
{
  if (id == 65535) return "";

  return cmdButton("icon" + std::to_string(id));
}

std::string icon(WeaponType const & t)
{
  return iconId(iconWeapon[t.getID()]);
}
std::string icon(UpgradeType const & t)
{
  return iconId(iconUpgrade[t.getID()]);
}
std::string icon(TechType const & t)
{
  return iconId(iconTech[t.getID()]);
}
std::string icon(Order const & t)
{
  return iconId(iconOrder[t.getID()]);
}
std::string icon(UnitType const & t)
{
  return iconId(t.getID());
}