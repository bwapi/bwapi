#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include "DevAIModule.h"

#define makelink(x) wiki(x.getName()).c_str(), x.getName().c_str()
#define maketitle(x) wiki(x.getName(), true).c_str()

void writeUnitWiki();
void writeWeaponWiki();
