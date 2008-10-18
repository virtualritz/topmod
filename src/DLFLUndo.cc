/*** ***/
/* $Id: DLFLUndo.cc,v 4.1 2004/02/24 20:41:44 vinod Exp $ */

#include "MainWindow.h"

//-- Subroutines dealing with undo and redo for DLFLWindow --//

void MainWindow::clearUndoList(void) {
  StringStreamPtrList::iterator first, firstmtl, last;
	firstmtl = undoMtlList.begin();
  first = undoList.begin(); last = undoList.end();
	while ( first != last ) {
		StringStreamPtr temp = (*first); ++first;
		StringStreamPtr tempmtl = (*firstmtl); ++firstmtl;
		delete temp;	delete tempmtl;
		temp = NULL; tempmtl = NULL;
	}
  undoList.clear();
  undoMtlList.clear();
}

void MainWindow::clearRedoList(void)
{
	StringStreamPtrList::iterator first, firstmtl, last;
	firstmtl = redoMtlList.begin();
	first = redoList.begin(); last = redoList.end();
	while ( first != last ) {
		StringStreamPtr temp = (*first); ++first;
		StringStreamPtr tempmtl = (*firstmtl); ++firstmtl;
		delete temp;	delete tempmtl;
		temp = NULL; tempmtl = NULL;
	}
	redoList.clear();
	redoMtlList.clear();
}

void MainWindow::undoPush(void)
{
     // Don't do anything unless undo is required
  if ( useUndo == false ) return;

     // Put current object on top of undo list
     // Check if we have reached undo limit, in which case remove oldest state
     // and add current state to end of list.
  if ( undoList.size() > undolimit ) {
		StringStreamPtr temp = undoList.front();
		StringStreamPtr tempmtl = undoMtlList.front();
		delete temp; delete tempmtl;
		undoList.pop_front();
  }

	StringStreamPtr curobj = new StringStream;
	StringStreamPtr curobjmtl = new StringStream;
	
	object.writeDLFL(*curobj, *curobjmtl);
	// object.writeMTL(*curobjmtl);
	
	undoList.push_back(curobj);
	undoMtlList.push_back(curobjmtl);
	// Evertime a new operation is done, previous state is put into UndoList
	// At the same time the redo list should be cleared, because we have
	// nothing to redo immediately after an operation.
	clearRedoList();
}

void MainWindow::undo(void) {
	
	if ( !undoList.empty() ) {		
 		// Restore previous object
		// Put current object to end of redo list
		// Take last element of undo list and re-create current object
		StringStreamPtr curobj = new StringStream;
		StringStreamPtr curobjmtl = new StringStream;

		object.writeDLFL(*curobj, *curobjmtl);
		// object.writeMTL(*curobjmtl);

		redoList.push_back(curobj);
		redoMtlList.push_back(curobjmtl);

		StringStreamPtr oldobj = undoList.back();
		StringStreamPtr oldobjmtl = undoMtlList.back();
		
		object.readDLFL(*oldobj,*oldobjmtl,true);
		// object.readMTL(*oldobj,true);

		undoList.pop_back(); delete oldobj;
		undoMtlList.pop_back(); delete oldobjmtl;
		
    cout << "Recompute patches for undo...." << endl;
		active->recomputePatches();
    cout << "done!" << endl;
		active->recomputeNormals();
		// Clear selection lists to avoid dangling pointers
		MainWindow::clearSelected();
		redraw();
		/* is document modified? - dave */
		setModified(true);
		mIsPrimitive = false; 
	}
	if (undoList.empty()) {
		setModified(false);
		if (mWasPrimitive)
			mIsPrimitive = true;
	}
}

void MainWindow::redo(void) {
	
  if ( !redoList.empty() ) {
		// Redo previously undone operation
		// Put current object to end of undo list
		// Take last element of redo list and re-create current object
		StringStreamPtr curobj = new StringStream;
		StringStreamPtr curobjmtl = new StringStream;
		object.writeDLFL(*curobj, *curobjmtl);
		undoList.push_back(curobj);
		undoMtlList.push_back(curobjmtl);

		StringStreamPtr newobj = redoList.back();
		StringStreamPtr newobjmtl = redoMtlList.back();
		object.readDLFL(*newobj, *newobjmtl);
		redoList.pop_back(); delete newobj;
		redoMtlList.pop_back(); delete newobjmtl;

		active->recomputePatches();
		active->recomputeNormals();
		// Clear selection lists to avoid dangling pointers
		MainWindow::clearSelected();
		redraw();
		/* is document modified? - dave */
		setModified(true);
		mIsPrimitive = false;
	}
}
