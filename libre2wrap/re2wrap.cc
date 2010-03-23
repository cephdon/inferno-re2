#include <re2/re2.h>
#include <stdio.h>
#include <malloc.h>

using namespace re2;

extern "C"
RE2* NewRE(const char* re, int* parens)
{
	RE2* pattern = new RE2(re);
	if(!pattern->error().empty()){
		delete pattern;
		return NULL;
	}
	*parens = pattern->NumberOfCapturingGroups();
	return pattern;
}

extern "C"
void DeleteRE(RE2* pattern)
{
	delete pattern;
}

extern "C"
int Match(const char* text, const RE2* re, int off[], int end[], int argc)
{
	StringPiece s = text;
	StringPiece parens[argc];
	int match, i;

	match = re->Match(s, 0, RE2::UNANCHORED, parens, argc);

	if(match)
		for(i = 0; i < argc; i++)
			if(parens[i].data() == NULL){ // no match
				off[i] = -1;
				end[i] = -1;
			} else { // match empty string if off[i]==end[i]
				off[i] = parens[i].data() - s.data();
				end[i] = off[i] + parens[i].size();
			}

	return match;
}

extern "C"
int Replace(char* str, const RE2* re, const char* rewrite)
{
}

