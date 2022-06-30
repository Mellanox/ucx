/*
* Copyright (c) 2019. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * See file LICENSE for terms.
 */
package org.openucx.jucx.ucp;

import org.openucx.jucx.UcxNativeStruct;

import java.net.InetSocketAddress;

/**
 * A server-side handle to incoming connection request. Can be used to create an
 * endpoint which connects back to the client.
 */
public class UcpConnectionRequest extends UcxNativeStruct {

    private InetSocketAddress clientAddress;

    /**
     * The address of the remote client that sent the connection request to the server.
     */
    public InetSocketAddress getClientAddress() {
        return clientAddress;
    }

    private UcpConnectionRequest(long nativeId, InetSocketAddress clientAddress) {
        setNativeId(nativeId);
        this.clientAddress = clientAddress;
    }
}
