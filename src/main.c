#include <adwaita.h>

static void
new_tab_cb (GtkButton *btn,
            AdwTabView *view)
{
  static int tab_count = 0;
  char *title = g_strdup_printf ("Tab %d", ++tab_count);

  GtkWidget *scrolled = gtk_scrolled_window_new ();
  GtkWidget *text_view = gtk_text_view_new ();
  gtk_text_view_set_left_margin (GTK_TEXT_VIEW (text_view), 12);
  gtk_text_view_set_right_margin (GTK_TEXT_VIEW (text_view), 12);
  gtk_text_view_set_top_margin (GTK_TEXT_VIEW (text_view), 12);
  gtk_text_view_set_bottom_margin (GTK_TEXT_VIEW (text_view), 12);

  GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
  char *content_text = g_strdup_printf ("This is the content of %s.\nNow in beautiful Libadwaita!", title);
  gtk_text_buffer_set_text (buffer, content_text, -1);
  g_free (content_text);

  gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scrolled), text_view);

  AdwTabPage *page = adw_tab_view_append (view, scrolled);
  adw_tab_page_set_title (page, title);

  adw_tab_view_set_selected_page (view, page);

  g_free (title);
}

static void
activate (AdwApplication *app)
{
  GtkWidget *window = adw_application_window_new (GTK_APPLICATION (app));
  gtk_window_set_title (GTK_WINDOW (window), "ChromeTabs Libadwaita");
  gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

  AdwToolbarView *toolbar_view = ADW_TOOLBAR_VIEW (adw_toolbar_view_new ());
  adw_application_window_set_content (ADW_APPLICATION_WINDOW (window), GTK_WIDGET (toolbar_view));

  AdwHeaderBar *header_bar = ADW_HEADER_BAR (adw_header_bar_new ());
  adw_toolbar_view_add_top_bar (toolbar_view, GTK_WIDGET (header_bar));

  AdwTabView *tab_view = ADW_TAB_VIEW (adw_tab_view_new ());
  adw_toolbar_view_set_content (toolbar_view, GTK_WIDGET (tab_view));

  AdwTabBar *tab_bar = ADW_TAB_BAR (adw_tab_bar_new ());
  adw_tab_bar_set_view (tab_bar, tab_view);
  adw_toolbar_view_add_top_bar (toolbar_view, GTK_WIDGET (tab_bar));

  GtkWidget *new_tab_btn = gtk_button_new_from_icon_name ("list-add-symbolic");
  gtk_widget_set_tooltip_text (new_tab_btn, "New Tab");
  adw_tab_bar_set_end_action_widget (tab_bar, new_tab_btn);
  g_signal_connect (new_tab_btn, "clicked", G_CALLBACK (new_tab_cb), tab_view);

  // Add initial tabs
  for (int i = 0; i < 3; i++) {
    new_tab_cb (NULL, tab_view);
  }

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int argc, char *argv[])
{
  AdwApplication *app = adw_application_new ("org.example.ChromeTabs", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  return g_application_run (G_APPLICATION (app), argc, argv);
}
