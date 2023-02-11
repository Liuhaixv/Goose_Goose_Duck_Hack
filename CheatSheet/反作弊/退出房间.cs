// Namespace: ExitGames.Client.Photon
public enum StatusCode // TypeDefIndex: 16118
{
    // Fields
    public int value__; // 0x0
public const StatusCode Connect = 1024;
public const StatusCode Disconnect = 1025;
public const StatusCode Exception = 1026;
public const StatusCode ExceptionOnConnect = 1023;
public const StatusCode ServerAddressInvalid = 1050;
public const StatusCode DnsExceptionOnConnect = 1051;
public const StatusCode SecurityExceptionOnConnect = 1022;
public const StatusCode SendError = 1030;
public const StatusCode ExceptionOnReceive = 1039;
public const StatusCode TimeoutDisconnect = 1040;
public const StatusCode DisconnectByServerTimeout = 1041;
public const StatusCode DisconnectByServerUserLimit = 1042;
public const StatusCode DisconnectByServerLogic = 1043;
public const StatusCode DisconnectByServerReasonUnknown = 1044;
public const StatusCode EncryptionEstablished = 1048;
public const StatusCode EncryptionFailedToEstablish = 1049;
}

void __stdcall Photon_Realtime_LoadBalancingClient__OnStatusChanged(
        Photon_Realtime_LoadBalancingClient_o *this,
        int32_t statusCode,//enum StatusCode
        const MethodInfo *method)