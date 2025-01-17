#pragma once
enum class ServerEnvironment {
    SC_PROD,
    NT_PROD,
    TX_PROD
};

class Starrpark {
    static inline ServerEnvironment ENV = ServerEnvironment::NT_PROD;
public:
    static bool isNullsBrawl() {
        return ENV == ServerEnvironment::NT_PROD;
    }
};