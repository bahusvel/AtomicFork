If parent dies it is detached from terminal, which is fine because thats what happens on commit() anyway. In fact in this case it should be better.

Child can kill parent so thats fine

Child needs to monitor parent or somehow be prevented from running while parent is alive... this is the difficult point... Simplest yet kinda dumb version is to just poll for it with kill(ppid, 0), which can potentially give me pretty good speed. And this one is POSIX thing I believe... Its questionable whether its worth it though. It does simplify the logic and improve performance though...

Kill loop is not such a bad option, I should speed test it vs the standard waitpid(), waitpid() is probably really fast though

