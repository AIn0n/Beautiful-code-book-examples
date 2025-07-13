	/** Set*editor and *edit_baton to a cancellation editor that
	 * wraps wrapped_editor and wrapped_baton.
	 *
	 * The editor will call cancel_func with cancel_baton when each of
	 * its functions is called, continuing on to call the corresponding wrapped
	 * function if cancel_func returns SVN_NO_ERROR.
	 *
	 * If cancel_func is NULL, set *editor to wrapped_editor and
	 * *edit_baton to wrapped_baton.
	 */
	svn_error_t *
	svn_delta_get_cancellation_editor(svn_cancel_func_t cancel_func,
	                                  void *cancel_baton,
	                                  const svn_delta_editor_t *wrapped_editor,
	                                  void *wrapped_baton,
	                                  const svn_delta_editor_t **editor,
	                                  void **edit_baton,
	                                  apr_pool_t *pool);