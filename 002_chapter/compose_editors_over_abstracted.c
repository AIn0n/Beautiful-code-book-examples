/** Compose editor_1 and its baton with editor_2 and its baton. 
	 *
	 * Return a new editor in new_editor (allocated in pool), in which
	 * each function fun calls editor_1->fun and then editor_2->fun,
	 * with the corresponding batons. 
	 *
	 * If editor_1->fun returns error, that error is returned from
	 * new_editor->fun and editor_2->fun is never called; otherwise
	 * new_editor->fun's return value is the same as editor_2->fun's.
	 *
	 * If an editor function is null, it is simply never called, and this
	 * is not an error.
	 */
	void
	svn_delta_compose_editors(const svn_delta_editor_t **new_editor,
	                          void **new_edit_baton,
	                          const svn_delta_editor_t *editor_1,
	                          void *edit_baton_1,
	                          const svn_delta_editor_t *editor_2,
	                          void *edit_baton_2,
	                          apr_pool_t *pool);