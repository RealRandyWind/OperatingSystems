#include <dirent.h>
#include "misc.h"

#define NOFINDS -1
#define BUFF_SIZE 512
#define INIT_MAP_SIZE 32
#define DUPLICATE_ROOT "."
#define DIRSEP "/"
#define ISHOMEDIR(a) (eq0(".",a))
#define ISPARENTDIR(a) (eq0("..",a))
#define DUPLICATE_MSG_SS "%s and %s are same file\n"

typedef struct file_t { char *name, *dir, *path; off_t size; } file_t;
typedef struct iter_t { char *dir; DIR *pdir; struct iter_t *_next; } iter_t;
typedef struct map_t { int N, _N; file_t *d; } map_t;

/* safely allocats a file array of size N */
file_t* file_safe(int N)
{
	return (file_t *) malloc_safe(N * sizeof(file_t));
}

/* safely reallocates a file array to size N */
file_t* file2_safe(file_t *d, int N)
{
	return (file_t *) realloc_safe(d, N * sizeof(file_t));
}

/* frees the file */
void freefile(file_t file)
{
	free_safe(file.name); free_safe(file.dir); free_safe(file.path);
}

/* frees the map and its files */
void freemap(map_t map)
{
	while(--map.N >= 0) { freefile(map.d[map.N]); }
	free_safe(map.d);
}

/* raw type constructor of map */
map_t rawmap(int N)
{
	map_t map;

	map.N = 0; map._N = N;
	map.d = file_safe(N);
	return map;
}

/* raw type constructor of it */
iter_t rawiter(const char *dir)
{
	iter_t it;

	it.pdir = nullptr; it._next = nullptr;
	it.dir = (char *) cp0len(nullptr, dir);
	return it;
}

/*
 * nextf reads the next file given an iterator into a file
 * the iterator holds the state of the read, it supports
 * for recursive.
 *
 * resolving of symbolic links could be done and and
 * repopulates the dir entry to the infor of the final file
 * linked.
 */
int nextf(iter_t *pit, file_t *pfile)
{
	iter_t it, it2, *pit2;
	file_t file;
	struct stat fstat;
	struct dirent* pdent;
	char* path;

	if(!pit || !pfile) { exit(EXIT_FAILURE); }
	it = *pit; path = nullptr;

	if(!it.pdir) { it.pdir = opendir(it.dir); }

	/* it0 = itk = {p0,d0,-} */
	while(it.pdir)
	{
		/* itk = {pk,dk,itn} */
		if(!(pdent = readdir(it.pdir)))
		{
			/* itk = {pk,dk,itn} */
			closedir(it.pdir); free_safe(it.dir);
			it.pdir = nullptr; it.dir = nullptr;
			if(!it._next) { break; } 
			/* itk = {-,-,itn} */
			pit2 = it._next; it = *it._next; free_safe(pit2);
			/* itk = {pn,-,it2n} */
			it.pdir = opendir(it.dir);
			/* itk = {pn,dn,it2n} */
			continue;
		}

		path = (char *) catd0len(it.dir, pdent->d_name, DIRSEP);
		stat_safe(path, &fstat);

		if(S_ISLNK(fstat.st_mode))
		{
			free_safe(path); path = nullptr;
			/* skip resolving for now, needs detection of circularity */
			continue;
			/*
			 * resolve link untill not link anymore
			 * continue if reaches nowhere or circualrity 
			 * flag circulary links in some way
			 *
			 * Not implemented because assumed not needed for assignment
			 */
		}

		/* itk = {pk, dk,itn}, pdk = {ik,rk,tk,nk} */
		if(S_ISDIR(fstat.st_mode))
		{
			if (ISHOMEDIR(pdent->d_name) || ISPARENTDIR(pdent->d_name)) { continue; }
			/* itk = {pk,dk,itn}, it2k = {-,-,-} */
			it2.dir = path; path = nullptr;
			/* itk = {pk,dk,itn}, it2k = {pk2,-,-} */
			it2._next = it._next;
			/* itk = {pk,dk,itn}, it2k = {pk2,-,itn} */
			it._next = (iter_t *) malloc_safe(sizeof(iter_t));
			*it._next = it2;
			/* itk = {pk,dk,it2k}, it2k = {pk2,d2k,itn} */
			continue;
		}

		if(S_ISREG(fstat.st_mode))
		{
			file.name = (char *) cp0len(nullptr, pdent->d_name);
			file.dir = (char *) cp0len(nullptr, it.dir);
			file.path = path; path = nullptr;
			file.size = fstat.st_size;
			*pit = it; *pfile = file;
			return true;
		}

		free_safe(path);
	}
	/* itk = {-,-,-} */
	return false;
}

/*
 * install puts a file into the map if not equal to any file
 * already in the map. it inserts in O(N) and compairs only
 * when file sizes are equivalent and allocats the buffers only
 * once.
 *
 * search could be done in faster than O(N) and buffers could
 * be reduced by only reading in a block size, and compare
 * these blocks until inequivalent, stack based buffers can be
 * used than.
 */
int install(file_t file, map_t *pmap)
{
	int n, fd, fdn;
	char *buff, *buffn;
	file_t filen;
	map_t map;
	
	if(!pmap) { exit(EXIT_FAILURE); }
	map = *pmap; fd = -1;

	for(n = 0; n < map.N; ++n)
	{
		filen = map.d[n];
		if(file.size != filen.size) { continue; }
		if(fd < 0)
		{
			/* once init buff and buffn if equal size */
			buff = char_safe(file.size);
			buffn = char_safe(file.size);
			/* once read file to buff for compare */
			fd = open_safe(file.path, O_RDONLY);
			read_safe(fd, buff, file.size);
		}
		/* open filen for compare compare and close it */
		fdn = open_safe(filen.path, O_RDONLY);
		read_safe(fdn, buffn, file.size);
		close_safe(fdn);
		/* return index of eq file if same */
		if(eq(buff, buffn, file.size)) { return n; }
	}

	/* free buff, buffn and close file */
	if(fd > 0) { close_safe(fd); free_safe(buff); free_safe(buffn); }

	/* increase size map if needed adding new file */
	if(map.N >= map._N) { map.d = file2_safe(map.d, map._N <<= 1); }

	map.d[map.N++] = file;

	*pmap = map;
	return NOFINDS;
}

int main()
{
	int n;
	file_t file;
	iter_t it;
	map_t map;

	/* init map and iterator */
	it = rawiter(DUPLICATE_ROOT);
	map = rawmap(INIT_MAP_SIZE);

	/* loop all files foud by iterator */
	while(nextf(&it, &file))
	{
		/* try and install the file when found or print duplicate and free file */
		n = install(file, &map);
		if(n >= 0) { printf(DUPLICATE_MSG_SS, map.d[n].path, file.path); freefile(file); } 
	}

	freemap(map);

	return EXIT_SUCCESS;
}