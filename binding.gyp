{
    "targets": [
        {
            "target_name": "mappo",
            "sources": [
                "src/mappo.cc",
                "src/map.cc",
                "src/mappo/map.cc"
            ],
            "include_dirs": [
                "/usr/local/Cellar/libagg/2.5/include/agg2"
            ],
            "libraries": [
                "-lpng"
            ]
        }
    ]
}