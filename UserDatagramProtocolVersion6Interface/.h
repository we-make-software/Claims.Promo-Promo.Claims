struct SKBDHCPv6 {
    u8 msg_type
    u8 transaction_id[3]
    u8 options[1024]
} __attribute__((packed))
