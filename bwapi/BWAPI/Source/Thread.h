#pragma once

void RegisterThreadName(const char *threadName, unsigned threadId = ~0);
void CheckRegisteredThreads();

void SetThreadName(const char *threadName, unsigned threadId = ~0);
