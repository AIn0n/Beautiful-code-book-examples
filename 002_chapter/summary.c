    svn_delta_editor_t *editor
	void *edit_baton;

	/* In real life, this would be a passed-in parameter, of course. */
	int summarize_only = TRUE;

	/* In real life, these variables would be declared in subroutines,
	   so that their lifetimes would be bound to the stack frame just
	   as the objects they point to are bound by the tree edit frame. */
	void *root_baton;
	void *dir_baton;
	void *subdir_baton;
	void *file_baton;

	/* Similarly, there would be subpools, not just one top-level pool. */
	apr_pool_t *pool = svn_pool_create();

	/* Each use of the delta editor interface starts by requesting the 
	   particular editor that implements whatever you need, e.g.,
	   streaming the edit across the network, applying it to a working
	   copy, etc. */
	Get_Update_Editor(&editor, &eb,
	                some_repository,
	                1, /* source revision number */
	                2, /* target revision number */
	                pool);

	/* Now we drive the editor. In real life, this sequence of calls
	   would be dynamically generated, by code that walks the two
	   repository trees and invokes editor->foo() as appropriate. */

	editor->open_root(edit_baton, pool, &root_baton);
	editor->open_directory("A", root_baton, pool, &dir_baton);
	editor->open_directory("A/fish", dir_baton,pool, &subdir_baton);
	editor->open_file("A/fish/tuna", subdir_baton, pool, &file_baton);

	if (! summarize_only)
	  {
	    svn_txdelta_window_handler_t window_handler;
	    void *window_handler_baton;
	    svn_txdelta_window_t *window;

	    editor->apply_textdelta(file_baton, pool
	                               apr_pool_t *pool,
	                               &window_handler,
	                               &window_handler_baton);
	   do {
	     window = Get_Next_TextDelta_Window(...);
	     window_handler(window, window_handler_baton);
	   } while (window);
	 }

	 editor->close_file(file_baton, pool);
	 editor->close_directory(subdir_baton, pool);
	 editor->close_directory(dir_baton, pool);
	 editor->close_directory(root_baton, pool);
	 editor->close_edit(edit_baton, pool);