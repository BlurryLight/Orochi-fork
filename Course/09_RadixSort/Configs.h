

constexpr auto WG_SIZE{ 64 };
constexpr auto N_RADIX{ 8 };
constexpr auto BIN_SIZE{ 1 << N_RADIX };
constexpr auto RADIX_MASK{ ( 1 << N_RADIX ) - 1 };

constexpr auto N_BINS_PER_WI{ BIN_SIZE / WG_SIZE };
constexpr auto N_BINS_4BIT{ 16 };
constexpr auto N_BINS_PACK_FACTOR{ sizeof( long long ) / sizeof( short ) };
constexpr auto N_BINS_PACKED_4BIT{ N_BINS_4BIT / N_BINS_PACK_FACTOR };

// count config
constexpr auto COUNT_WG_SIZE{ BIN_SIZE };

// scan configs
constexpr auto SCAN_WG_SIZE{ BIN_SIZE };

// sort configs
constexpr auto SORT_WG_SIZE{ 64 };
constexpr auto SORT_N_ITEMS_PER_WI{ 12 };
constexpr auto SINGLE_SORT_N_ITEMS_PER_WI{ 24 };
constexpr auto SINGLE_SORT_WG_SIZE{ 128 };
