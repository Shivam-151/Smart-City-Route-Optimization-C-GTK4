# Editable diagrams

These Mermaid diagrams can be pasted into a Mermaid-compatible editor (for example, Mermaid Live Editor) and exported for the report.

## System flow
```mermaid
flowchart TD
    A([Start]) --> B[Open GTK4 window]
    B --> C[Select source and destination]
    C --> D[Select algorithm]
    D --> E{Traffic enabled?}
    E -->|Yes| F[Apply sample road penalties]
    E -->|No| G[Use base road weights]
    F --> H[Run selected graph algorithm]
    G --> H
    H --> I{Route found?}
    I -->|Yes| J[Show path, cost and visited count]
    I -->|No| K[Show no-route message]
    J --> L[Highlight route on graph]
    K --> M([Wait for next query])
    L --> M
```

## High-level class/module view
```mermaid
classDiagram
    class CityGraph {
      +places
      +coordinates
      +roadWeights
      +edgeCost(a,b,traffic)
    }
    class RouteResult {
      +path
      +length
      +cost
      +visited_count
      +found
    }
    class RouteAlgorithms {
      +dijkstra(start,goal,traffic)
      +astar(start,goal,traffic)
      +bfs(start,goal)
      +dfs(start,goal)
    }
    class GTKInterface {
      +selectLocations()
      +selectAlgorithm()
      +runSearch()
      +drawMap()
      +showResult()
    }
    GTKInterface --> CityGraph
    GTKInterface --> RouteAlgorithms
    RouteAlgorithms --> CityGraph
    RouteAlgorithms ..> RouteResult
```
