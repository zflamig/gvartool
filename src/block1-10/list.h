#ifndef _LIST_H_
#define _LIST_H_

struct LISTITEM
{
  LineDoc *lineDoc;
  int line;

  LISTITEM *next;
  static LISTITEM *trash;

  LISTITEM(void);
  LISTITEM(LineDoc *ld);
  ~LISTITEM(void);
  void *operator new(size_t size);
  void operator delete(void *it, size_t size);

  LISTITEM *add(LISTITEM *item);
  void discard(void);
  int operator<=(LineDoc *ld);
  int operator<=(LISTITEM *li);

  static void emptyTrash(void);
};

struct LISTHEAD
{
  LISTITEM *head;

  LISTHEAD(LISTITEM *item = 0);
  ~LISTHEAD(void);
  void empty(void);
  void operator<<(LISTITEM *item);
  void operator--(int);
  void flushIt(void);
  void writeImagerLine(LineDoc *lineDoc);
};

#endif // list.h
