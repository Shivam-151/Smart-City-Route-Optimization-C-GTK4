#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define N 7
#define INF 1000000000

/* A small fictional city. Coordinates are used only by A* as a heuristic. */
static const char *places[N] = {
    "Central Station", "City Hospital", "City Mall", "River Bridge",
    "University", "Tech Park", "Airport Road"
};
static const double xy[N][2] = {
    {0, 0}, {1, 3}, {2, 1}, {4, 2}, {3, 5}, {6, 4}, {7, 1}
};

/* Base road distances in kilometres; 0 means no direct road. */
static const int base[N][N] = {
    {0,4,2,0,0,0,0},
    {4,0,1,5,3,0,0},
    {2,1,0,3,0,6,0},
    {0,5,3,0,2,2,7},
    {0,3,0,2,0,3,2},
    {0,0,6,2,3,0,4},
    {0,0,0,7,2,4,0}
};

typedef struct {
    int path[N + 1];
    int length;
    int cost;
    int visited_count;
    gboolean found;
} RouteResult;

static GtkWidget *window, *source_combo, *dest_combo, *algorithm_combo;
static GtkWidget *traffic_check, *result_label, *map_area, *comparison_label;
static int current_path[N + 1];
static int current_path_len = 0;

static int edge_cost(int a, int b, gboolean traffic) {
    int w = base[a][b];
    if (w && traffic) {
        /* Simple demonstration: selected busy roads receive a fixed penalty. */
        if ((a == 1 && b == 3) || (a == 3 && b == 1) ||
            (a == 2 && b == 5) || (a == 5 && b == 2)) w += 5;
    }
    return w;
}

static double heuristic(int a, int b) {
    double dx = xy[a][0] - xy[b][0];
    double dy = xy[a][1] - xy[b][1];
    return sqrt(dx * dx + dy * dy);
}

static RouteResult empty_result(void) {
    RouteResult r;
    memset(&r, 0, sizeof(r));
    r.cost = INF;
    return r;
}

static RouteResult dijkstra(int start, int goal, gboolean traffic) {
    RouteResult r = empty_result();
    int dist[N], prev[N], used[N] = {0};
    for (int i = 0; i < N; i++) { dist[i] = INF; prev[i] = -1; }
    dist[start] = 0;
    for (int step = 0; step < N; step++) {
        int u = -1;
        for (int i = 0; i < N; i++)
            if (!used[i] && (u == -1 || dist[i] < dist[u])) u = i;
        if (u == -1 || dist[u] == INF) break;
        used[u] = 1; r.visited_count++;
        if (u == goal) break;
        for (int v = 0; v < N; v++) {
            int w = edge_cost(u, v, traffic);
            if (w && !used[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w; prev[v] = u;
            }
        }
    }
    if (dist[goal] == INF) return r;
    int rev[N], count = 0;
    for (int at = goal; at != -1 && count < N; at = prev[at]) rev[count++] = at;
    r.length = count;
    for (int i = 0; i < count; i++) r.path[i] = rev[count - 1 - i];
    r.cost = dist[goal]; r.found = TRUE;
    return r;
}

static RouteResult astar(int start, int goal, gboolean traffic) {
    RouteResult r = empty_result();
    double g[N]; int prev[N], closed[N] = {0};
    for (int i = 0; i < N; i++) { g[i] = 1e30; prev[i] = -1; }
    g[start] = 0;
    for (int step = 0; step < N; step++) {
        int u = -1; double best = 1e30;
        for (int i = 0; i < N; i++) {
            if (!closed[i] && g[i] < 1e29) {
                double f = g[i] + heuristic(i, goal);
                if (f < best) { best = f; u = i; }
            }
        }
        if (u == -1) break;
        closed[u] = 1; r.visited_count++;
        if (u == goal) break;
        for (int v = 0; v < N; v++) {
            int w = edge_cost(u, v, traffic);
            if (w && !closed[v] && g[u] + w < g[v]) {
                g[v] = g[u] + w; prev[v] = u;
            }
        }
    }
    if (g[goal] >= 1e29) return r;
    int rev[N], count = 0;
    for (int at = goal; at != -1 && count < N; at = prev[at]) rev[count++] = at;
    r.length = count;
    for (int i = 0; i < count; i++) r.path[i] = rev[count - 1 - i];
    r.cost = (int)round(g[goal]); r.found = TRUE;
    return r;
}

static RouteResult bfs(int start, int goal) {
    RouteResult r = empty_result();
    int queue[N], head = 0, tail = 0, prev[N], seen[N] = {0};
    for (int i = 0; i < N; i++) prev[i] = -1;
    queue[tail++] = start; seen[start] = 1;
    while (head < tail) {
        int u = queue[head++]; r.visited_count++;
        if (u == goal) break;
        for (int v = 0; v < N; v++) {
            if (base[u][v] && !seen[v]) {
                seen[v] = 1; prev[v] = u; queue[tail++] = v;
            }
        }
    }
    if (!seen[goal]) return r;
    int rev[N], count = 0;
    for (int at = goal; at != -1 && count < N; at = prev[at]) rev[count++] = at;
    r.length = count;
    for (int i = 0; i < count; i++) r.path[i] = rev[count - 1 - i];
    r.cost = count - 1; r.found = TRUE; /* BFS cost is number of roads, not km. */
    return r;
}

static RouteResult dfs(int start, int goal) {
    RouteResult r = empty_result();
    int stack[N], top = 0, prev[N], seen[N] = {0};
    for (int i = 0; i < N; i++) prev[i] = -1;
    stack[top++] = start; seen[start] = 1;
    while (top) {
        int u = stack[--top];
        r.visited_count++;
        if (u == goal) break;
        /* Mark when pushing to avoid duplicate stack entries. */
        for (int v = N - 1; v >= 0; v--) {
            if (base[u][v] && !seen[v]) {
                seen[v] = 1; prev[v] = u; stack[top++] = v;
            }
        }
    }
    if (!seen[goal]) return r;
    int rev[N], count = 0;
    for (int at = goal; at != -1 && count < N; at = prev[at]) rev[count++] = at;
    r.length = count;
    for (int i = 0; i < count; i++) r.path[i] = rev[count - 1 - i];
    r.cost = count - 1; r.found = TRUE; /* DFS cost is number of roads. */
    return r;
}

static void draw_map(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
    (void)area; (void)data;
    cairo_set_source_rgb(cr, 0.97, 0.98, 0.99); cairo_paint(cr);
    double px[N], py[N];
    for (int i = 0; i < N; i++) {
        px[i] = 30 + xy[i][0] * (width - 60) / 8.0;
        py[i] = height - 25 - xy[i][1] * (height - 50) / 6.0;
    }
    /* Roads */
    for (int i = 0; i < N; i++) for (int j = i + 1; j < N; j++) {
        if (!base[i][j]) continue;
        cairo_set_source_rgb(cr, 0.72, 0.75, 0.79); cairo_set_line_width(cr, 2);
        cairo_move_to(cr, px[i], py[i]); cairo_line_to(cr, px[j], py[j]); cairo_stroke(cr);
        char label[16]; snprintf(label, sizeof(label), "%d", base[i][j]);
        cairo_set_source_rgb(cr, 0.35, 0.38, 0.42);
        cairo_move_to(cr, (px[i]+px[j])/2, (py[i]+py[j])/2); cairo_show_text(cr, label);
    }
    /* Selected route */
    if (current_path_len > 1) {
        cairo_set_source_rgb(cr, 0.10, 0.42, 0.72); cairo_set_line_width(cr, 4);
        cairo_move_to(cr, px[current_path[0]], py[current_path[0]]);
        for (int i = 1; i < current_path_len; i++)
            cairo_line_to(cr, px[current_path[i]], py[current_path[i]]);
        cairo_stroke(cr);
    }
    /* Place nodes and labels */
    for (int i = 0; i < N; i++) {
        cairo_set_source_rgb(cr, 1, 1, 1); cairo_arc(cr, px[i], py[i], 8, 0, 6.283185307179586); cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0.16, 0.25, 0.34); cairo_set_line_width(cr, 1.5); cairo_stroke(cr);
        cairo_set_source_rgb(cr, 0.10, 0.13, 0.17);
        cairo_move_to(cr, px[i] + 10, py[i] - 8); cairo_show_text(cr, places[i]);
    }
}

static void run_search(GtkButton *button, gpointer data) {
    (void)button; (void)data;
    int start = gtk_combo_box_get_active(GTK_COMBO_BOX(source_combo));
    int goal = gtk_combo_box_get_active(GTK_COMBO_BOX(dest_combo));
    int alg = gtk_combo_box_get_active(GTK_COMBO_BOX(algorithm_combo));
    gboolean traffic = gtk_check_button_get_active(GTK_CHECK_BUTTON(traffic_check));
    if (start < 0 || goal < 0 || alg < 0) {
        gtk_label_set_text(GTK_LABEL(result_label), "Please select a source, destination, and algorithm.");
        return;
    }
    RouteResult r;
    const char *alg_name;
    if (alg == 0) { r = dijkstra(start, goal, traffic); alg_name = "Dijkstra"; }
    else if (alg == 1) { r = astar(start, goal, traffic); alg_name = "A*"; }
    else if (alg == 2) { r = bfs(start, goal); alg_name = "BFS"; }
    else { r = dfs(start, goal); alg_name = "DFS"; }

    current_path_len = 0;
    if (!r.found) {
        gtk_label_set_text(GTK_LABEL(result_label), "No route found between the selected locations.");
        gtk_widget_queue_draw(map_area);
        return;
    }
    char path_text[1024] = "";
    for (int i = 0; i < r.length; i++) {
        current_path[i] = r.path[i];
        char part[128];
        snprintf(part, sizeof(part), "%s%s", i ? " → " : "", places[r.path[i]]);
        strncat(path_text, part, sizeof(path_text) - strlen(path_text) - 1);
    }
    current_path_len = r.length;
    char summary[1600];
    if (alg < 2)
        snprintf(summary, sizeof(summary), "%s route\n%s\nTotal distance: %d km\nNodes visited: %d%s",
                 alg_name, path_text, r.cost, r.visited_count, traffic ? "\nTraffic adjustment: ON" : "");
    else
        snprintf(summary, sizeof(summary), "%s traversal route\n%s\nRoads used: %d\nNodes visited: %d\nNote: BFS/DFS do not optimize distance.",
                 alg_name, path_text, r.cost, r.visited_count);
    gtk_label_set_text(GTK_LABEL(result_label), summary);
    gtk_widget_queue_draw(map_area);
}

static GtkWidget *make_combo(const char *items[], int count) {
    GtkWidget *combo = gtk_combo_box_text_new();
    for (int i = 0; i < count; i++) gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), items[i]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    return combo;
}

static void activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Smart City Route Optimization");
    gtk_window_set_default_size(GTK_WINDOW(window), 980, 680);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(root, 14); gtk_widget_set_margin_bottom(root, 14);
    gtk_widget_set_margin_start(root, 16); gtk_widget_set_margin_end(root, 16);
    gtk_window_set_child(GTK_WINDOW(window), root);

    GtkWidget *heading = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(heading), "<span size='x-large' weight='bold'>Smart City Route Optimization and Intelligent Navigation System</span>");
    gtk_label_set_xalign(GTK_LABEL(heading), 0);
    gtk_box_append(GTK_BOX(root), heading);

    GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_append(GTK_BOX(root), controls);
    const char *algorithms[] = {"Dijkstra (shortest distance)", "A* (heuristic search)", "BFS (fewest roads)", "DFS (traversal)"};
    source_combo = make_combo(places, N); dest_combo = make_combo(places, N);
    gtk_combo_box_set_active(GTK_COMBO_BOX(dest_combo), N-1);
    algorithm_combo = make_combo(algorithms, 4);
    gtk_box_append(GTK_BOX(controls), gtk_label_new("From:"));
    gtk_box_append(GTK_BOX(controls), source_combo);
    gtk_box_append(GTK_BOX(controls), gtk_label_new("To:"));
    gtk_box_append(GTK_BOX(controls), dest_combo);
    gtk_box_append(GTK_BOX(controls), algorithm_combo);
    traffic_check = gtk_check_button_new_with_label("Traffic");
    gtk_box_append(GTK_BOX(controls), traffic_check);
    GtkWidget *run = gtk_button_new_with_label("Find Route");
    g_signal_connect(run, "clicked", G_CALLBACK(run_search), NULL);
    gtk_box_append(GTK_BOX(controls), run);

    GtkWidget *map_frame = gtk_frame_new("City graph (edge labels show base distance in km)");
    map_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(map_area, 650, 360);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(map_area), draw_map, NULL, NULL);
    gtk_frame_set_child(GTK_FRAME(map_frame), map_area);
    gtk_box_append(GTK_BOX(root), map_frame);

    result_label = gtk_label_new("Choose a route and press Find Route. Traffic adds a fixed delay to two sample roads.");
    gtk_label_set_xalign(GTK_LABEL(result_label), 0);
    gtk_label_set_wrap(GTK_LABEL(result_label), TRUE);
    gtk_box_append(GTK_BOX(root), result_label);

    comparison_label = gtk_label_new(
        "Algorithm guide: Dijkstra = shortest weighted distance; A* = weighted search guided by a geometric estimate; "
        "BFS = fewest edges in an unweighted view; DFS = explores a branch before backtracking. "
        "Visited-node count is a simple demonstration metric, not a universal speed ranking.");
    gtk_label_set_xalign(GTK_LABEL(comparison_label), 0);
    gtk_label_set_wrap(GTK_LABEL(comparison_label), TRUE);
    gtk_box_append(GTK_BOX(root), comparison_label);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("edu.student.smartcityroutes", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
