/*
	Copyright 2005 Oliver Tappe
	
	Distributed under the MIT License
*/

#ifndef CDOC_IO_H
#define CDOC_IO_H

#include "CFtpStream.h"
#include "utf-support.h"

class CDoc;

class CDocIO
{
public:
			CDocIO(CDoc* doc=NULL);
	virtual ~CDocIO();

	virtual bool ReadDoc(bool readAttributes) = 0;
	virtual bool WriteDoc() = 0;

	virtual bool IsLocal() const = 0;
	virtual const char* Name() const = 0;
	virtual const entry_ref* EntryRef() const = 0;
	virtual const URLData* URL() const = 0;
			time_t LastSaved() const;

	virtual	void StartWatchingFile(void);
	virtual	void StopWatchingFile(bool stopDirectory = true);
	virtual void HandleNodeMonitorMsg(BMessage* msg);
	virtual bool VerifyFile();
	virtual void SetEntryRef(const entry_ref* ref);
	virtual BLooper* Target();
protected:
			bool ReadDocFromStream(BPositionIO& stream, bool readAttributes);
			bool DoPreEditTextConversions(BString& docText);
			bool DoPostEditTextConversions(BString& docText);

			CDoc* fDoc;
			CTextEncodingConverter fConv;
			time_t fLastSaved;
};

inline time_t CDocIO::LastSaved() const
{
	return fLastSaved;
}

class CLocalDocIO : public CDocIO
{
public:
			CLocalDocIO(CDoc* doc, const entry_ref* eref, BLooper* target);
	virtual	~CLocalDocIO();

	virtual bool ReadDoc(bool readAttributes);
	virtual bool WriteDoc();

	virtual bool IsLocal() const;
	virtual const char* Name() const;
	virtual const entry_ref* EntryRef() const;
	virtual const URLData* URL() const;

	virtual	void StartWatchingFile(void);
	virtual	void StopWatchingFile(bool stopDirectory = true);
	virtual void HandleNodeMonitorMsg(BMessage* msg);
	virtual bool VerifyFile();
	virtual void SetEntryRef(const entry_ref* ref);
	virtual BLooper* Target();
private:
			entry_ref *fEntryRef;
			node_ref fNodeRef;
			BLooper* fTarget;
};

class CFtpDocIO : public CDocIO
{
public:
			CFtpDocIO(CDoc* doc, const URLData& url);
	virtual	~CFtpDocIO();

	virtual bool ReadDoc(bool readAttributes);
	virtual bool WriteDoc();
	virtual bool IsLocal() const;

	virtual const char* Name() const;
	virtual const entry_ref* EntryRef() const;
	virtual const URLData* URL() const;
private:
			URLData *fURL;
};

#endif // CDOC_IO_H