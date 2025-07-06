/** A structure full of callback functions the delta source will invoke
	 * as it produces the delta.
	 *
	 * Function Usage
	 * ============== 
	 *
	 * Here's how to use these functions to express a tree delta.
	 *
	 * The delta consumer implements the callback functions described in
     * this structure, and thedelta producer invokes them. So the
	 * caller (producer) is pushing tree delta data at the callee
	 * (consumer).
	 *
	 * At the start of traversal, the consumer provides edit_baton, a
	 * baton global to the entire delta edit. 
	 *
	 * Next, if there are any tree deltas to express, the producer should
	 * pass the edit_baton to the open_root function, to get a baton
	 * representing root of the tree being edited.
	 *
	 * Most of the callbacks work in the obvious way:
	 *
	 * delete_entry
	 *  add_file
	 * add_directory
	 * open_file 
	 * open_directory
	 *
	 * Each of these takes a directory baton, indicating the directory
	 * in which the change takes place, and a path argument, giving the
	 * path (relative to the root of the edit) of the file,
	 * subdirectory, or directory entry to change. Editors will usually
	 * want to join this relative path with some base stored in the edit
	 * baton (e.g. a URL, a location in the OS filesystem). 
     *
	 * Since every call requires a parent directory baton, including
	 * add_directory and open_directory, where do we ever get our
	 * initial directory baton, to get things started? The open_root
	 * function returns a baton for the top directory of the change. In
	 * general, the producer needs to invoke the editor's open_root
	 * function before it can get anything of interest done.
	 *
	 * While open_root provides a directory baton for the root of
	 * the tree being changed, the add_directory and open_directory
	 * callbacks provide batons for other directories. Like the
	 * callbacks above, they take a parent_baton and a relative path
	 * path, and then return a new baton for the subdirectory being
	 * created / modified --- child_baton. The producer can then use
	 * child_baton to make further changes in that subdirectory. 
	 *
	 * So, if we already have subdirectories named `foo' and `foo/bar',
	 * then the producer can create a new file named `foo/bar/baz.c' by
	 * calling:
	 *
	 *     - open_root () --- yielding a baton root for the top directory
	 *
	 *     - open_directory (root, "foo")
	 *
	 *     - open_directory (f, "foo/bar") --- yielding a baton b for `foo/bar' 
	 *
	 *     - add_file (b, "foo/bar/baz.c")
	 *
     * When the producer is finished making changes to a directory, it
	 * should call close_directory. This lets the consumer do any
	 * necessary cleanup, and free the baton's storage.
	 *
	 * The add_file and open_file callbacks each ret urn a baton
	 * for the file being created or changed. This baton can then be
	 * passed to apply_textdelta to change the file's contents.
	 * When the producer is finished making changes to a file, it should
	 * call close_file, to let the consumer clean up and free the baton. 
	 *
	 * Function Call Ordering
	 * ======================
	 *
	 * There are five restrictions on the order in which the producer
	 * may use the batons: 
	 *
	 * 1. The producer may call open_directory, add_directory,
	 *    open_file, add_file at most once on any given directory
	 *	  entry. delete_entry may be called at most once on any given
	 *    directory entry and may later be followed by add_directory or
	 *	 add_file on the same directory entry. delete_entry may
	 *    not be called on any directory entry after open_directory,
	 *   add_directory,open_file or add_file has been called on
	 *    that directory entry.
	 *
	 * 2. The producer may not close a directory baton until it has
	 *    closed all batons for its subdirectories. 
	 *
	 * 3. When a producer calls open_directory or add_directory,
	 *    it must specify the most recently opened of the currently open
	 *    directory batons. Put another way, the producer cannot have
	 *    two sibling directory batons open at the same time.
	 *
	 * 4. When the producer calls open_file or add_file, it must
	 *    follow with any changes to the file (using apply_textdelta),
	 *    followed by a close_file call, before issuing any other
	 *    file or directory calls.
	 *
	 * 5. When the producer calls apply_textdelta, it must make all of
	 *    the window handler calls (including the NULL window at the
	 *    end) before issuing any other svn_delta_editor_t calls. 
	 *
	 * So, the producer needs to use directory and file batons as if it
	 * is doing a single depth-first traversal of the tree. 
	 *
	 * Pool Usage
	 * ==========
	 *
	 * Many editor functions are invoked multiple times, in a sequence
	 * determined by the editor "driver". The driver is responsible for
	 * creating a pool for use on each iteration of the editor function,
	 * and clearing that pool between each iteration. The driver passes
	 * the appropriate pool on each function invocation. 
	 *
	 * Based on the requirement of calling the editor functions in a
	 * depth-first style, it is usually customary for the driver to similarly
	 * nest the pools. However, this is only a safety feature to ensure
	 * that pools associated with deeper items are always cleared when the
	 * top-level items are also cleared. The interface does not assume, nor
	 * require, any particular organization of the pools passed to these
	 * functions.
	 */
	typedef struct svn_delta_editor_t 
	{
	  /** Set *root_baton to a baton for the top directory of the change.
	   * (This is the top of the subtree being changed, not necessarily
	   * the root of the filesystem.) Like any other directory baton, the
	   * producer should call close_directory on root_baton when they're
	   * done. 
	   */
	  svn_error_t *(*open_root)(void *edit_baton,
	                            apr_pool_t *dir_pool,
	                            void **root_baton);



	  /** Remove the directory entry named path, a child of the directory
	   * represented by parent_baton.
	   */
	  svn_error_t *(*delete_entry)(const char *path,
	                               void *parent_baton,
	                               apr_pool_t *pool);


// TBD