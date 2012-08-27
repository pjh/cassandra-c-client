/* cassandra_client.c - API for basic Cassandra C Client.
 *
 * Written in 2012 by Peter Hornyack <pjh@cs.washington.edu>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the public
 * domain worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

/* See the README file for instructions and required dependencies.
 * This simple client is intended only to interact with a single
 * Cassandra node running on localhost, treating it as just a basic
 * key-value store. None of the powerful features of the Cassandra
 * data model/API are used.
 *
 * This client library should follow these tips, but probably doesn't:
 * https://git.kernel.org/?p=linux/kernel/git/kay/libabc.git;a=blob_plain;f=README
 */

#ifndef CASSANDRA_CLIENT_H
#define CASSANDRA_CLIENT_H

#define CASSANDRA_PORT_DEFAULT 9160  // port for Thrift to listen for clients on

/* See description of these data models in cassandra_client.c: */
enum _DataModel {
	DM_ROWKEYS_USECOLVALS,
	DM_ROWKEYS_USECOLNAMES,
	DM_COLNAMEKEYS,
};
typedef enum _DataModel DataModel;

/* Opaque identifier for the Cassandra node that this client interacts
 * with.
 */
struct cassandra_node_struct;
typedef struct cassandra_node_struct cassandra_node;

/* Starts a Cassandra node running on the localhost, at the specified
 * port. Also performs initialization tasks, such as connecting to a
 * keyspace and creating a column family.
 * Returns: 0 on success, -1 on error. On success, *node is set to point
 * to a newly-allocated cassandra_node.
 */
int cassandra_start(cassandra_node **node, unsigned int port);
int cassandra_worker_start(cassandra_node **node, unsigned int port);

/* Performs a no-op Cassandra command, which can be used to measure the
 * overhead of the entire Cassandra connection chain (cassandra_client,
 * Thrift, network). Actually, these are not truly no-ops, but they
 * ask Cassandra to describe some very basic info. Specifically:
 *   noop1: cassandra_if_describe_version()
 *   noop2: cassandra_if_describe_cluster_name()
 *   noop3: cassandra_if_describe_partitioner()
 *   noop4: cassandra_if_describe_snitch()
 *   noop5: cassandra_if_describe_keyspaces()
 * Returns: 0 on success, -1 on error.
 */
int cassandra_noop1(cassandra_node *node);
int cassandra_noop2(cassandra_node *node);
int cassandra_noop3(cassandra_node *node);
int cassandra_noop4(cassandra_node *node);
int cassandra_noop5(cassandra_node *node);

/* Stops a Cassandra node running on localhost at the specified port. Also
 * performs cleanup tasks, such as removing the created column family. The
 * cassandra_node passed as an argument here is freed.
 * Returns: 0 on success, -1 on error.
 */
int cassandra_stop(cassandra_node *node);
int cassandra_worker_stop(cassandra_node *node);

/* Puts a key-value pair into the store. Both the key and the value must
 * be a null-terminated string. The caller can free these strings after
 * the function has returned.
 * Returns: 0 on success, -1 on error.
 */
int cassandra_put(cassandra_node *node, const char *key, const char *value);
int cassandra_put_noop(cassandra_node *node, const char *key, const char *value);

/* Gets a key-value pair from the store. The key should be a
 * null-terminated string. The caller is responsible for freeing key and
 * *value after this function returns.
 * Returns: 0 on success, 1 if key not found, -1 on error.
 */
int cassandra_get(cassandra_node *node, const char *key, char **value);

/* NOT IMPLEMENTED YET!
 * Deletes a key-value pair from the store. The key must be a
 * null-terminated string.
 * Returns: 0 on success, 1 if key not found, -1 on error.
 */
int cassandra_delete(cassandra_node *node, const char *key);

#endif  //CASSANDRA_CLIENT_H

