#include <fcntl.h>
#include <dirent.h>
#include "misc.h"

#define NOFINDS -1
#define BUFF_SIZE 512
#define INIT_MAP_SIZE 32
#define DUPLICATE_ROOT "."
#define DIRSEP "/"
#define ISHOMEDIR(a) (eq0(".",a))
#define ISPARENTDIR(a) (eq0("..",a))
#define DT_ISLNK(a) ((a) == DT_LNK)
#define DT_ISREG(a) ((a) == DT_REG)
#define DT_ISDIR(a) ((a) == DT_DIR)
#define DUPLICATE_MSG_SS "%s and %s are same file\n"

typedef struct file_t { char *name, *dir, *path; off_t size; } file_t;
typedef struct iter_t { char *dir; DIR *pdir; struct iter_t *_next; } iter_t;
typedef struct map_t { int N, _N; file_t *d; } map_t;

/* safely allocats a file array of size N */
file_t* file_s(int N)
{
	return (file_t *) malloc_s(N * sizeof(file_t));
}

/* safely reallocates a file array to size N */
file_t* file2_s(file_t *d, int N)
{
	return (file_t *) realloc_s(d, N * sizeof(file_t));
}

/* frees the file */
void freefile(file_t file)
{
	free_s(file.name); free_s(file.dir); free_s(file.path);
}

/* frees the map and its files */
void freemap(map_t map)
{
	while(--map.N >= 0) { freefile(map.d[map.N]); }
	free_s(map.d);
}

/* raw type constructor of map */
map_t rawmap(int N)
{
	map_t map;

	map.N = 0; map._N = N;
	map.d = file_s(N);
	return map;
}

/* raw type constructor of it */
iter_t rawit(const char *dir)
{
	iter_t it;

	it.pdir = nullptr; it._next = nullptr;
	it.dir = cp0len(nullptr, dir);
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
	const char *path;
	DIR* pdir;
	iter_t it, it2, *pit2;
	file_t file;
	struct stat fstat;
	struct dirent* pdent;

	if(!pit || !pfile) { exit(EXIT_FAILURE); }
	it = *pit; /*_perv = pit;*/

	if(!it.pdir) { it.pdir = opendir(it.dir); }

	/* it0 = itk = {p0,d0,-} */
	while(it.pdir)
	{
		/* itk = {pk,dk,itn} */
		if(!(pdent = readdir(it.pdir)))
		{
			/* itk = {pk,dk,itn} */
			closedir(it.pdir); free_s(it.dir);
			it.pdir = nullptr; it.dir = nullptr;
			if(!it._next) { break; } 
			/* itk = {-,-,itn} */
			pit2 = it._next; it = *it._next; free_s(pit2);
			/* itk = {pn,-,it2n} */
			it.pdir = opendir(it.dir);
			/* itk = {pn,dn,it2n} */
			continue;
		}

		if(DT_ISLNK(pdent->d_type))
		{
			/* skip resolving for now, needs detection of circularity */
			continue;
			/*
			 * resolve link untill not link anymore
			 * continue if reaches nowhere or circualrity 
			 * flag circulary links in some way
			 */
		}

		/* itk = {pk, dk,itn}, pdk = {ik,rk,tk,nk} */
		if(DT_ISDIR(pdent->d_type))
		{
			if (ISHOMEDIR(pdent->d_name) || ISPARENTDIR(pdent->d_name)) { continue; }
			/* itk = {pk,dk,itn}, it2k = {-,-,-} */
			it2.dir = catd0len(it.dir, pdent->d_name, DIRSEP);
			/* itk = {pk,dk,itn}, it2k = {pk2,-,-} */
			it2._next = it._next;
			/* itk = {pk,dk,itn}, it2k = {pk2,-,itn} */
			it._next = (iter_t *) malloc(sizeof(iter_t));
			*it._next = it2;
			/* itk = {pk,dk,it2k}, it2k = {pk2,d2k,itn} */
			continue;
		}

		if(DT_ISREG(pdent->d_type))
		{
			file.name = cp0len(nullptr, pdent->d_name);
			file.dir = cp0len(nullptr, it.dir);
			file.path = catd0len(file.dir, file.name, DIRSEP);
			stat_s(file.path, &fstat);
			file.size = fstat.st_size;
			*pit = it; *pfile = file;
			return true;
		}
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
			buff = char_s(file.size);
			buffn = char_s(file.size);
			/* once read file to buff for compare */
			fd = open(file.path, O_RDONLY);
			read(fd, buff, file.size);
		}
		/* open filen for compare compare and close it */
		fdn = open(filen.path, O_RDONLY);
		read(fdn, buffn, file.size);
		close(fdn);
		/* return index of eq file if same */
		if(eq(buff, buffn, file.size)) { return n; }
	}

	/* free buff, buffn and close file */
	if(fd > 0) { close(fd); free_s(buff); free_s(buffn); }

	/* increase size map if needed adding new file */
	if(map.N >= map._N) { map.d = file2_s(map.d, map._N <<= 1); }

	map.d[map.N++] = file;

	*pmap = map;
	return NOFINDS;
}

/*S_ISLNK, S_ISREG, S_ISDIR, S_IFLNK*/
/* open(.., O_RDONLY)*/
int main(int argc, char **argv, char **envp)
{
	int n;
	file_t file;
	iter_t it;
	map_t map;

	if(argc != 1) { return EXIT_FAILURE; }
	
	it = rawit(DUPLICATE_ROOT);
	map = rawmap(INIT_MAP_SIZE);

	while(nextf(&it, &file))
	{
		n = install(file, &map);
		if(n >= 0) { printf(DUPLICATE_MSG_SS, map.d[n].path, file.path); freefile(file); } 
	}

	freemap(map);

	return EXIT_SUCCESS;
}