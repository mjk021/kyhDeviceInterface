/*
 * opensc.h: OpenSC library header file
 *
 * Copyright (C) 2001, 2002  Juha Yrjola<juha.yrjola@iki.fi>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** 
 * @file opensc.h
 * @brief OpenSC library core header file
 */
 
#ifndef _OPENSC_H
#define _OPENSC_H

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#include <opensc/scconf.h>
#include <opensc/errors.h>
#include <opensc/types.h>

#ifndef __GNUC__
#undef inline
#define inline
#endif

/* Different APDU cases */
#define SC_APDU_CASE_NONE		0
#define SC_APDU_CASE_1                  1
#define SC_APDU_CASE_2_SHORT            2
#define SC_APDU_CASE_3_SHORT            3
#define SC_APDU_CASE_4_SHORT            4
#define SC_APDU_CASE_2_EXT              5
#define SC_APDU_CASE_3_EXT              6
#define SC_APDU_CASE_4_EXT              7

/* File types */
#define SC_FILE_TYPE_DF			0x04
#define SC_FILE_TYPE_INTERNAL_EF	0x03
#define SC_FILE_TYPE_WORKING_EF		0x01

/* EF structures */
#define SC_FILE_EF_UNKNOWN		0x00
#define SC_FILE_EF_TRANSPARENT		0x01
#define SC_FILE_EF_LINEAR_FIXED		0x02
#define SC_FILE_EF_LINEAR_FIXED_TLV	0x03
#define SC_FILE_EF_LINEAR_VARIABLE	0x04
#define SC_FILE_EF_LINEAR_VARIABLE_TLV	0x05
#define SC_FILE_EF_CYCLIC		0x06
#define SC_FILE_EF_CYCLIC_TLV		0x07

/* File status flags */
#define SC_FILE_STATUS_ACTIVATED	0x00
#define SC_FILE_STATUS_INVALIDATED	0x01

/* Access Control flags */
#define SC_AC_NONE			0x00000000
#define SC_AC_CHV			0x00000001 /* Card Holder Verif. */
#define SC_AC_TERM			0x00000002 /* Terminal auth. */
#define SC_AC_PRO			0x00000004 /* Secure Messaging */
#define SC_AC_AUT			0x00000008 /* Key auth. */

#define SC_AC_SYMBOLIC			0x00000010 /* internal use only */
#define SC_AC_UNKNOWN			0xFFFFFFFE
#define SC_AC_NEVER			0xFFFFFFFF

/* Operations relating to access control (in case of DF) */
#define SC_AC_OP_SELECT			0
#define SC_AC_OP_LOCK			1
#define SC_AC_OP_DELETE			2
#define SC_AC_OP_CREATE			3
#define SC_AC_OP_REHABILITATE		4
#define SC_AC_OP_INVALIDATE		5
#define SC_AC_OP_LIST_FILES		6
#define SC_AC_OP_CRYPTO			7
/* If you add more OPs here, make sure you increase
 * SC_MAX_AC_OPS in types.h */
 
/* Operations relating to access control (in case of EF) */
#define SC_AC_OP_READ			0
#define SC_AC_OP_UPDATE			1
#define SC_AC_OP_WRITE			2
#define SC_AC_OP_ERASE			3
/* rehab and invalidate are the same as in DF case */

/* sc_*_record() flags */
#define SC_RECORD_EF_ID_MASK		0x0001F
#define SC_RECORD_BY_REC_ID		0x00000
#define SC_RECORD_BY_REC_NR		0x00100
#define SC_RECORD_CURRENT		0

/* various maximum values */
#define SC_MAX_CARD_DRIVERS		16
#define SC_MAX_READER_DRIVERS		4
#define SC_MAX_CARD_DRIVER_SNAME_SIZE	16
#define SC_MAX_READERS			16
#define SC_MAX_SLOTS			4
#define SC_MAX_CARD_APPS		8
#define SC_MAX_APDU_BUFFER_SIZE		258
#define SC_MAX_PIN_SIZE			256 /* OpenPGP card has 254 max */
#define SC_MAX_ATR_SIZE			33
#define SC_MAX_AID_SIZE			16
/* Beware: the following needs to be a mutiple of 4
 * or else sc_update_binary will not work on GPK */
#define SC_APDU_CHOP_SIZE		248

#define SC_AC_KEY_REF_NONE	0xFFFFFFFF

#define SC_SEC_OPERATION_DECIPHER	0x0001
#define SC_SEC_OPERATION_SIGN		0x0002
#define SC_SEC_OPERATION_AUTHENTICATE	0x0003

/* sc_security_env flags */
#define SC_SEC_ENV_ALG_REF_PRESENT	0x0001
#define SC_SEC_ENV_FILE_REF_PRESENT	0x0002
#define SC_SEC_ENV_KEY_REF_PRESENT	0x0004
/* FIXME: the flag below is misleading */
#define SC_SEC_ENV_KEY_REF_ASYMMETRIC	0x0008
#define SC_SEC_ENV_ALG_PRESENT		0x0010

/* PK algorithms */
#define SC_ALGORITHM_RSA		0
#define SC_ALGORITHM_DSA		1
#define SC_ALGORITHM_EC			2

/* Symmetric algorithms */
#define SC_ALGORITHM_DES		64
#define SC_ALGORITHM_3DES		65

/* Hash algorithms */
#define SC_ALGORITHM_MD5		128
#define SC_ALGORITHM_SHA1		129

/* Key derivation algorithms */
#define SC_ALGORITHM_PBKDF2		192

/* Key encryption algoprithms */
#define SC_ALGORITHM_PBES2		256

#define SC_ALGORITHM_ONBOARD_KEY_GEN	0x80000000
#define SC_ALGORITHM_NEED_USAGE		0x40000000
#define SC_ALGORITHM_SPECIFIC_FLAGS	0x0000FFFF

#define SC_ALGORITHM_RSA_RAW		0x00000001
/* If the card is willing to produce a cryptogram padded with the following 
 * methods, set these flags accordingly. */
#define SC_ALGORITHM_RSA_PADS		0x0000000E
#define SC_ALGORITHM_RSA_PAD_NONE	0x00000000
#define SC_ALGORITHM_RSA_PAD_PKCS1	0x00000002
#define SC_ALGORITHM_RSA_PAD_ANSI	0x00000004
#define SC_ALGORITHM_RSA_PAD_ISO9796	0x00000008

/* If the card is willing to produce a cryptogram with the following 
 * hash values, set these flags accordingly. */
#define SC_ALGORITHM_RSA_HASH_NONE	0x00000010
#define SC_ALGORITHM_RSA_HASHES		0x000001E0
#define SC_ALGORITHM_RSA_HASH_SHA1	0x00000020
#define SC_ALGORITHM_RSA_HASH_MD5	0x00000040
#define SC_ALGORITHM_RSA_HASH_MD5_SHA1	0x00000080
#define SC_ALGORITHM_RSA_HASH_RIPEMD160	0x00000100

/* A 64-bit uint, used in sc_current_time() */
#ifndef _WIN32
typedef unsigned long long sc_timestamp_t;
#define msleep(t)      usleep((t) * 1000)
#else
typedef unsigned __int64 sc_timestamp_t;
#define msleep(t)      Sleep(t)
#define sleep(t)       Sleep((t) * 1000)
#endif

/* Event masks for sc_wait_for_event() */
#define SC_EVENT_CARD_INSERTED		0x0001
#define SC_EVENT_CARD_REMOVED		0x0002

struct sc_security_env {
	unsigned long flags;
	int operation;
	unsigned int algorithm, algorithm_flags;

	unsigned int algorithm_ref;
	struct sc_path file_ref;
	u8 key_ref[8];
	size_t key_ref_len;
};
typedef struct sc_security_env sc_security_env_t;

struct sc_algorithm_id {
	unsigned int algorithm;
	struct sc_object_id obj_id;
	void *params;
};

struct sc_pbkdf2_params {
	u8 salt[16];
	size_t salt_len;
	int iterations;
	size_t key_length;
	struct sc_algorithm_id hash_alg;
};

struct sc_pbes2_params {
	struct sc_algorithm_id derivation_alg;
	struct sc_algorithm_id key_encr_alg;
};

struct sc_algorithm_info {
	unsigned int algorithm;
	unsigned int key_length;
	unsigned long flags;

	union {
		struct sc_rsa_info {
			long exponent;
		} _rsa;
	} u;
};
typedef struct sc_algorithm_info sc_algorithm_info_t;

struct sc_app_info {
	u8 aid[SC_MAX_AID_SIZE];
	size_t aid_len;
	char *label;
	struct sc_path path;
	u8 *ddo;
	size_t ddo_len;
	
	const char *desc;	/* App description, if known */
	int rec_nr;		/* -1, if EF(DIR) is transparent */
};
typedef struct sc_app_info sc_app_info_t;

struct sc_card_cache {
	struct sc_path current_path;
};

#define SC_PROTO_T0		0x00000001
#define SC_PROTO_T1		0x00000002
#define SC_PROTO_RAW		0x00001000
#define SC_PROTO_ANY		0xFFFFFFFF

struct sc_reader_driver {
	const char *name;
	const char *short_name;
	struct sc_reader_operations *ops;

	size_t max_send_size, max_recv_size;
	int apdu_masquerade;
	void *dll;
};
#define SC_APDU_MASQUERADE_NONE		0x00
#define SC_APDU_MASQUERADE_4AS3		0x01
#define SC_APDU_MASQUERADE_1AS2		0x02
#define SC_APDU_MASQUERADE_1AS2_ALWAYS	0x04

/* slot flags */
#define SC_SLOT_CARD_PRESENT	0x00000001
#define SC_SLOT_CARD_CHANGED	0x00000002
/* slot capabilities */
#define SC_SLOT_CAP_DISPLAY	0x00000001
#define SC_SLOT_CAP_PIN_PAD	0x00000002

struct sc_slot_info {
	int id;	
	unsigned long flags, capabilities;
	unsigned int supported_protocols, active_protocol;
	u8 atr[SC_MAX_ATR_SIZE];
	size_t atr_len;

	struct _atr_info {
		u8 *hist_bytes;
		size_t hist_bytes_len;
		int Fi, f, Di, N;
		u8 FI, DI;
	} atr_info;

	void *drv_data;
};
typedef struct sc_slot_info sc_slot_info_t;

struct sc_event_listener {
	unsigned int event_mask;
	void (*func)(void *, const struct sc_slot_info *, unsigned int event);
};

struct sc_reader {
	struct sc_context *ctx;
	const struct sc_reader_driver *driver;
	const struct sc_reader_operations *ops;
	void *drv_data;
	char *name;

	struct sc_slot_info slot[SC_MAX_SLOTS];
	int slot_count;
};
typedef struct sc_reader sc_reader_t;

/* This will be the new interface for handling PIN commands.
 * It is supposed to support pin pads (with or without display)
 * attached to the reader.
 */
#define SC_PIN_CMD_VERIFY	0
#define SC_PIN_CMD_CHANGE	1
#define SC_PIN_CMD_UNBLOCK	2

#define SC_PIN_CMD_USE_PINPAD	0x0001
#define SC_PIN_CMD_NEED_PADDING	0x0002

#define SC_PIN_ENCODING_ASCII	0
#define SC_PIN_ENCODING_BCD	1
#define SC_PIN_ENCODING_GLP	2 /* Global Platform - Card Specification ?v 2.0.1 */

struct sc_pin_cmd_pin {
	const char *prompt;	/* Prompt to display */

	const u8 *data;		/* PIN, if given by the appliction */
	int len;		/* set to -1 to get pin from pin pad */

	size_t min_length;	/* min/max length of PIN */
	size_t max_length;
	int encoding;		/* ASCII-numeric, BCD, etc */
	size_t pad_length;	/* filled in by the card driver */
	u8 pad_char;
	size_t offset;          /* PIN offset in the APDU */
	size_t length_offset;	/* Effective PIN length offset in the APDU */
};

struct sc_pin_cmd_data {
	unsigned int cmd;
	unsigned int flags;

	unsigned int pin_type;		/* usually SC_AC_CHV */
	int pin_reference;

	struct sc_pin_cmd_pin pin1, pin2;

	struct sc_apdu *apdu;		/* APDU of the PIN command */
};

#define SC_DISCONNECT			0
#define SC_DISCONNECT_AND_RESET		1
#define SC_DISCONNECT_AND_UNPOWER	2
#define SC_DISCONNECT_AND_EJECT		3

struct sc_reader_operations {
	/* Called during sc_establish_context(), when the driver
	 * is loaded */
	int (*init)(struct sc_context *ctx, void **priv_data);
	/* Called when the driver is being unloaded.  finish() has to
	 * deallocate the private data and any resources. */
	int (*finish)(struct sc_context *ctx, void *priv_data);
	/* Called when releasing a reader.  release() has to
	 * deallocate the private data.  Other fields will be
	 * freed by OpenSC. */
	int (*release)(struct sc_reader *reader);

	int (*detect_card_presence)(struct sc_reader *reader,
				    struct sc_slot_info *slot);
	int (*connect)(struct sc_reader *reader, struct sc_slot_info *slot);
	int (*disconnect)(struct sc_reader *reader, struct sc_slot_info *slot,
			  int action);
	int (*transmit)(struct sc_reader *reader, struct sc_slot_info *slot,
			const u8 *sendbuf, size_t sendsize,
			u8 *recvbuf, size_t *recvsize,
			unsigned long control);
	int (*lock)(struct sc_reader *reader, struct sc_slot_info *slot);
	int (*unlock)(struct sc_reader *reader, struct sc_slot_info *slot);
	int (*set_protocol)(struct sc_reader *reader, struct sc_slot_info *slot,
			    unsigned int proto);
	/* Pin pad functions */
	int (*display_message)(struct sc_reader *, struct sc_slot_info *,
			       const char *);
	int (*perform_verify)(struct sc_reader *, struct sc_slot_info *,
			 struct sc_pin_cmd_data *);

	/* Wait for an event */
	int (*wait_for_event)(struct sc_reader **readers, 
			      struct sc_slot_info **slots,
			      size_t nslots,
			      unsigned int event_mask,
			      int *reader_index,
			      unsigned int *event,
			      int timeout);
};

/* Mutexes - this is just a dummy struct used for type
 * safety; internally we use objects defined by the
 * underlying thread model
 */
typedef struct sc_mutex sc_mutex_t;

struct sc_mutex *sc_mutex_new(void);
void sc_mutex_lock(struct sc_mutex *p);
void sc_mutex_unlock(struct sc_mutex *p);
void sc_mutex_free(struct sc_mutex *p);

/*
 * Card flags
 */
/* none yet */

/*
 * Card capabilities 
 */
/* SC_CARD_APDU_EXT: Card can handle large (> 256 bytes) buffers in
 * calls to read_binary, write_binary and update_binary; if not,
 * several successive calls to the corresponding function is made. */
#define SC_CARD_CAP_APDU_EXT		0x00000001
/* SC_CARD_CAP_EMV: Card can handle operations specified in the
 * EMV 4.0 standard. */
#define SC_CARD_CAP_EMV			0x00000002
/* SC_CARD_CAP_RNG: Card has on-board random number source */
#define SC_CARD_CAP_RNG			0x00000004

struct sc_card {
	struct sc_context *ctx;
	struct sc_reader *reader;
	struct sc_slot_info *slot;

	unsigned long caps, flags;
	int cla;
	u8 atr[SC_MAX_ATR_SIZE];
	size_t atr_len;
	size_t max_send_size;
	size_t max_recv_size;

	struct sc_app_info *app[SC_MAX_CARD_APPS];
	int app_count;
	struct sc_file *ef_dir;
	
	struct sc_algorithm_info *algorithms;
	int algorithm_count;
	
	int lock_count;

	struct sc_card_driver *driver;
	struct sc_card_operations *ops;
	const char *name;
	void *drv_data;
	int max_pin_len;

	struct sc_card_cache cache;
	int cache_valid;

	sc_mutex_t *mutex;

	unsigned int magic;
};
typedef struct sc_card sc_card_t;

struct sc_card_operations {
	/* Called in sc_connect_card().  Must return 1, if the current
	 * card can be handled with this driver, or 0 otherwise.  ATR
	 * field of the sc_card struct is filled in before calling
	 * this function. */
	int (*match_card)(struct sc_card *card);

	/* Called when ATR of the inserted card matches an entry in ATR
	 * table.  May return SC_ERROR_INVALID_CARD to indicate that
	 * the card cannot be handled with this driver. */
	int (*init)(struct sc_card *card);
	/* Called when the card object is being freed.  finish() has to
	 * deallocate all possible private data. */
	int (*finish)(struct sc_card *card);

	/* ISO 7816-4 functions */

	int (*read_binary)(struct sc_card *card, unsigned int idx,
			   u8 * buf, size_t count, unsigned long flags);
	int (*write_binary)(struct sc_card *card, unsigned int idx,
			    const u8 * buf, size_t count, unsigned long flags);
	int (*update_binary)(struct sc_card *card, unsigned int idx,
			     const u8 * buf, size_t count, unsigned long flags);
	int (*erase_binary)(struct sc_card *card, unsigned int idx,
			    size_t count, unsigned long flags);

	int (*read_record)(struct sc_card *card, unsigned int rec_nr,
			   u8 * buf, size_t count, unsigned long flags);
	int (*write_record)(struct sc_card *card, unsigned int rec_nr,
			    const u8 * buf, size_t count, unsigned long flags);
	int (*append_record)(struct sc_card *card, const u8 * buf,
			     size_t count, unsigned long flags);
	int (*update_record)(struct sc_card *card, unsigned int rec_nr,
			     const u8 * buf, size_t count, unsigned long flags);

	/* select_file: Does the equivalent of SELECT FILE command specified
	 *   in ISO7816-4. Stores information about the selected file to
	 *   <file>, if not NULL. */
	int (*select_file)(struct sc_card *card, const struct sc_path *path,
			   struct sc_file **file_out);
	int (*get_response)(struct sc_card *card, sc_apdu_t *orig_apdu, size_t count);
	int (*get_challenge)(struct sc_card *card, u8 * buf, size_t count);

	/*
	 * ISO 7816-8 functions
	 */

	/* verify:  Verifies reference data of type <acl>, identified by
	 *   <ref_qualifier>. If <tries_left> is not NULL, number of verifying
	 *   tries left is saved in case of verification failure, if the
	 *   information is available. */
	int (*verify)(struct sc_card *card, unsigned int type,
		      int ref_qualifier, const u8 *data, size_t data_len,
		      int *tries_left);

	/* logout: Resets all access rights that were gained. */
	int (*logout)(struct sc_card *card);

	/* restore_security_env:  Restores a previously saved security
	 *   environment, and stores information about the environment to
	 *   <env_out>, if not NULL. */
	int (*restore_security_env)(struct sc_card *card, int se_num);

	/* set_security_env:  Initializes the security environment on card
	 *   according to <env>, and stores the environment as <se_num> on the
	 *   card. If se_num <= 0, the environment will not be stored. */
	int (*set_security_env)(struct sc_card *card,
			        const struct sc_security_env *env, int se_num);
	/* decipher:  Engages the deciphering operation.  Card will use the
	 *   security environment set in a call to set_security_env or
	 *   restore_security_env. */
	int (*decipher)(struct sc_card *card, const u8 * crgram,
		        size_t crgram_len, u8 * out, size_t outlen);
	
	/* compute_signature:  Generates a digital signature on the card.  Similiar
	 *   to the function decipher. */
	int (*compute_signature)(struct sc_card *card, const u8 * data,
				 size_t data_len, u8 * out, size_t outlen);
	int (*change_reference_data)(struct sc_card *card, unsigned int type,
				     int ref_qualifier,
				     const u8 *old, size_t oldlen,
				     const u8 *newref, size_t newlen,
				     int *tries_left);
	int (*reset_retry_counter)(struct sc_card *card, unsigned int type,
				   int ref_qualifier,
				   const u8 *puk, size_t puklen,
				   const u8 *newref, size_t newlen);
	/*
	 * ISO 7816-9 functions
	 */
	int (*create_file)(struct sc_card *card, struct sc_file *file);
	int (*delete_file)(struct sc_card *card, const struct sc_path *path);
	/* list_files:  Enumerates all the files in the current DF, and
	 *   writes the corresponding file identifiers to <buf>.  Returns
	 *   the number of bytes stored. */
	int (*list_files)(struct sc_card *card, u8 *buf, size_t buflen);
	
	int (*check_sw)(struct sc_card *card, int sw1, int sw2);
	int (*card_ctl)(struct sc_card *card, unsigned long request,
				void *data);
	int (*process_fci)(struct sc_card *card, struct sc_file *file,
			const u8 *buf, size_t buflen);
	int (*construct_fci)(struct sc_card *card, const struct sc_file *file,
			u8 *out, size_t *outlen);

	/* pin_cmd: verify/change/unblock command; optionally using the
	 * card's pin pad if supported.
	 */
	int (*pin_cmd)(struct sc_card *, struct sc_pin_cmd_data *,
				int *tries_left);

	int (*get_data)(sc_card_t *, unsigned int, u8 *, size_t);
	int (*put_data)(sc_card_t *, unsigned int, const u8 *, size_t);

	int (*delete_record)(sc_card_t *card, unsigned int rec_nr);
};

struct sc_card_driver {
	const char *name;
	const char *short_name;
	struct sc_card_operations *ops;
	struct sc_atr_table *atr_map;
	unsigned int natrs;
	void *dll;
};

struct sc_context {
	scconf_context *conf;
	scconf_block *conf_blocks[3];
	char *app_name;
	int debug;

	int suppress_errors;
	FILE *debug_file, *error_file;
	char *preferred_language;

	const struct sc_reader_driver *reader_drivers[SC_MAX_READER_DRIVERS+1];
	void *reader_drv_data[SC_MAX_READER_DRIVERS];
	
	struct sc_reader *reader[SC_MAX_READERS];
	int reader_count;
	
	struct sc_card_driver *card_drivers[SC_MAX_CARD_DRIVERS+1];
	struct sc_card_driver *forced_driver;

	sc_mutex_t *mutex;

	unsigned int magic;
};
typedef struct sc_context sc_context_t;

/* Base64 encoding/decoding functions */
int sc_base64_encode(const u8 *in, size_t inlen, u8 *out, size_t outlen,
		     size_t linelength);
int sc_base64_decode(const char *in, u8 *out, size_t outlen);

/* Returns the current time in milliseconds */
sc_timestamp_t sc_current_time(void);

/* APDU handling functions */
int sc_transmit_apdu(struct sc_card *card, struct sc_apdu *apdu);
void sc_format_apdu(struct sc_card *card, struct sc_apdu *apdu, int cse, int ins,
		    int p1, int p2);

/**
 * Establishes an OpenSC context
 * @param ctx A pointer to a pointer that will receive the allocated context
 * @param app_name A string that identifies the application, used primarily
 *	in finding application-specific configuration data. Can be NULL.
 */
int sc_establish_context(struct sc_context **ctx, const char *app_name);

/**
 * Releases an established OpenSC context
 * @param ctx A pointer to the context structure to be released
 */
int sc_release_context(struct sc_context *ctx);
/**
 * Forces the use of a specified card driver
 * @param ctx OpenSC context
 * @param short_name The short name of the driver to use (e.g. 'emv')
 */
int sc_set_card_driver(struct sc_context *ctx, const char *short_name);
/**
 * Connects to a card in a reader and auto-detects the card driver.
 * The ATR (Answer to Reset) string of the card is also retrieved.
 * @param reader Reader structure
 * @param slot_id Slot ID to connect to
 * @param card The allocated card object will go here */
int sc_connect_card(struct sc_reader *reader, int slot_id,
		    struct sc_card **card);
/**
 * Disconnects from a card, and frees the card structure. Any locks
 * made by the application must be released before calling this function.
 * NOTE: The card is not reset nor powered down after the operation.
 * @param card The card to disconnect
 */
int sc_disconnect_card(struct sc_card *card, int action);
/**
 * Returns 1 if the magic value of the card object is correct. Mostly
 * used internally by the library.
 * @param card The card object to check
 */
inline int sc_card_valid(const struct sc_card *card);

/**
 * Checks if a card is present in a reader
 * @param reader Reader structure
 * @param slot_id Slot ID
 * @retval If an error occured, the return value is a (negative)
 *	OpenSC error code. If no card is present, 0 is returned.
 *	Otherwise, a positive value is returned, which is a
 *	combination of flags. The flag SC_SLOT_CARD_PRESENT is
 *	always set. In addition, if the card was exchanged,
 *	the SC_SLOT_CARD_CHANGED flag is set.
 */
int sc_detect_card_presence(struct sc_reader *reader, int slot_id);

/**
 * Waits for an event on readers. Note: only the event is detected,
 * there is no update of any card or other info.
 * @param readers array of pointer to a Reader structure
 * @param reader_count amount of readers in the array
 * @param slot_id Slot ID
 * @param event_mask The types of events to wait for; this should
 *   be ORed from one of the following
 *   	SC_EVENT_CARD_REMOVED
 *   	SC_EVENT_CARD_INSERTED
 * @param reader (OUT) the reader on which the event was detected
 * @param event (OUT) the events that occurred. This is also ORed
 *   from the SC_EVENT_CARD_* constants listed above.
 * @param timeout Amount of millisecs to wait; -1 means forever
 * @retval < 0 if an error occured
 * @retval = 0 if a an event happened
 * @retval = 1 if the timeout occured
 */
int sc_wait_for_event(struct sc_reader **readers, int *slots, size_t nslots,
                      unsigned int event_mask,
                      int *reader, unsigned int *event, int timeout);

/**
 * Locks the card against modification from other threads.
 * After the initial call to sc_lock, the card is protected from
 * access from other processes. The function may be called several times.
 * @param card The card to lock
 * @retval SC_SUCCESS on success
 */
int sc_lock(struct sc_card *card);
/** 
 * Unlocks a previously locked card. After the lock count drops to zero,
 * the card is again placed in shared mode, where other processes
 * may access or lock it.
 * @param card The card to unlock
 * @retval SC_SUCCESS on success
 */
int sc_unlock(struct sc_card *card);

/* ISO 7816-4 related functions */

/**
 * Does the equivalent of ISO 7816-4 command SELECT FILE.
 * @param card The card on which to issue the command
 * @param path The path, file id or name of the desired file
 * @param file If not NULL, will receive a pointer to a new structure
 * @retval SC_SUCCESS on success
 */
int sc_select_file(struct sc_card *card, const struct sc_path *path,
		   struct sc_file **file);

int sc_list_files(struct sc_card *card, u8 * buf, size_t buflen);

/* TODO: finish writing API docs */
int sc_read_binary(struct sc_card *card, unsigned int idx, u8 * buf,
		   size_t count, unsigned long flags);
int sc_write_binary(struct sc_card *card, unsigned int idx, const u8 * buf,
		    size_t count, unsigned long flags);
int sc_update_binary(struct sc_card *card, unsigned int idx, const u8 * buf,
		     size_t count, unsigned long flags);
/**
 * Reads a record from the current (i.e. selected) file.
 * @param card The card on which to issue the command
 * @param rec_nr SC_READ_RECORD_CURRENT or a record number starting from 1
 * @param buf Pointer to a buffer for storing the data
 * @param count Number of bytes to read
 * @param flags Flags
 * @retval Number of bytes read or an error value
 */
int sc_read_record(struct sc_card *card, unsigned int rec_nr, u8 * buf,
		   size_t count, unsigned long flags);
int sc_write_record(struct sc_card *card, unsigned int rec_nr, const u8 * buf,
		    size_t count, unsigned long flags);
int sc_append_record(struct sc_card *card, const u8 * buf, size_t count,
		     unsigned long flags);
int sc_update_record(struct sc_card *card, unsigned int rec_nr, const u8 * buf,
		     size_t count, unsigned long flags);
int sc_delete_record(struct sc_card *card, unsigned int rec_nr);

/* get/put data functions */
int sc_get_data(sc_card_t *, unsigned int, u8 *, size_t);
int sc_put_data(sc_card_t *, unsigned int, const u8 *, size_t);

int sc_get_challenge(struct sc_card *card, u8 * rndout, size_t len);

/* ISO 7816-8 related functions */
int sc_restore_security_env(struct sc_card *card, int se_num);
int sc_set_security_env(struct sc_card *card,
			const struct sc_security_env *env, int se_num);
int sc_decipher(struct sc_card *card, const u8 * crgram, size_t crgram_len,
		u8 * out, size_t outlen);
int sc_compute_signature(struct sc_card *card, const u8 * data,
			 size_t data_len, u8 * out, size_t outlen);
int sc_verify(struct sc_card *card, unsigned int type, int ref, const u8 *buf,
	      size_t buflen, int *tries_left);
int sc_logout(struct sc_card *card);
int sc_pin_cmd(struct sc_card *card, struct sc_pin_cmd_data *, int *tries_left);
int sc_change_reference_data(struct sc_card *card, unsigned int type,
			     int ref, const u8 *old, size_t oldlen,
			     const u8 *newref, size_t newlen,
			     int *tries_left);
int sc_reset_retry_counter(struct sc_card *card, unsigned int type,
			   int ref, const u8 *puk, size_t puklen,
			   const u8 *newref, size_t newlen);
int sc_build_pin(u8 *buf, size_t buflen, struct sc_pin_cmd_pin *pin, int pad);
/* pkcs1 padding/encoding functions */
int sc_pkcs1_add_01_padding(const u8 *in, size_t in_len, u8 *out,
			    size_t *out_len, size_t mod_length);
int sc_pkcs1_strip_01_padding(const u8 *in_dat, size_t in_len, u8 *out_dat,
			      size_t *out_len);
int sc_pkcs1_strip_02_padding(const u8 *data, size_t len, u8 *out_dat,
			      size_t *out_len);
int sc_pkcs1_add_digest_info_prefix(unsigned int algorithm, const u8 *in_dat,
		size_t in_len, u8 *out_dat, size_t *out_len);
int sc_pkcs1_strip_digest_info_prefix(unsigned int *algorithm,
		const u8 *in_dat, size_t in_len, u8 *out_dat, size_t *out_len);
int sc_pkcs1_encode(struct sc_context *ctx, unsigned long flags,
	const u8 *in, size_t in_len, u8 *out, size_t *out_len, size_t mod_len);
int sc_strip_zero_padding(const u8 *in,size_t in_len, u8 *out, size_t *out_len);
/* ISO 7816-9 */
int sc_create_file(struct sc_card *card, struct sc_file *file);
int sc_delete_file(struct sc_card *card, const struct sc_path *path);

/* Card controls */
int sc_card_ctl(struct sc_card *card, unsigned long command, void *arg);

inline int sc_file_valid(const struct sc_file *file);
struct sc_file * sc_file_new(void);
void sc_file_free(struct sc_file *file);
void sc_file_dup(struct sc_file **dest, const struct sc_file *src);

int sc_file_add_acl_entry(struct sc_file *file, unsigned int operation,
			  unsigned int method, unsigned long key_ref);
const struct sc_acl_entry * sc_file_get_acl_entry(const struct sc_file *file,
						  unsigned int operation);
void sc_file_clear_acl_entries(struct sc_file *file, unsigned int operation);

int sc_file_set_sec_attr(struct sc_file *file, const u8 *sec_attr,
			 size_t sec_attr_len);
int sc_file_set_prop_attr(struct sc_file *file, const u8 *prop_attr,
			  size_t prop_attr_len);
int sc_file_set_type_attr(struct sc_file *file, const u8 *type_attr,
			  size_t type_attr_len);

void sc_format_path(const char *path_in, struct sc_path *path_out);
const char *sc_print_path(const sc_path_t *path_in);
int sc_compare_path(const sc_path_t *, const sc_path_t *);
int sc_append_path(struct sc_path *dest, const struct sc_path *src);
int sc_append_path_id(struct sc_path *dest, const u8 *id, size_t idlen);
int sc_append_file_id(struct sc_path *dest, unsigned int fid);
int sc_hex_to_bin(const char *in, u8 *out, size_t *outlen);
int sc_bin_to_hex(const u8 *, size_t, char *, size_t, char separator);

int sc_get_cache_dir(struct sc_context *ctx, char *buf, size_t bufsize);
int sc_make_cache_dir(struct sc_context *ctx);

int sc_enum_apps(struct sc_card *card);
void sc_free_apps(struct sc_card *card);
const struct sc_app_info * sc_find_pkcs15_app(struct sc_card *card);
const struct sc_app_info * sc_find_app_by_aid(struct sc_card *card,
					      const u8 *aid, size_t aid_len);
int sc_update_dir(struct sc_card *card, struct sc_app_info *app);

struct sc_card_error {
	int SWs;
	int errorno;
	const char *errorstr;
};

extern const char *sc_get_version(void);

#define SC_IMPLEMENT_DRIVER_VERSION(a) \
	static const char *drv_version = (a); \
	const char *sc_driver_version()\
	{ \
		return drv_version; \
	}

extern struct sc_reader_driver *sc_get_pcsc_driver(void);
extern struct sc_reader_driver *sc_get_ctapi_driver(void);
extern struct sc_reader_driver *sc_get_openct_driver(void);

extern struct sc_card_driver *sc_get_default_driver(void);
extern struct sc_card_driver *sc_get_emv_driver(void);
extern struct sc_card_driver *sc_get_etoken_driver(void);
extern struct sc_card_driver *sc_get_cryptoflex_driver(void);
extern struct sc_card_driver *sc_get_cyberflex_driver(void);
extern struct sc_card_driver *sc_get_gpk_driver(void);
extern struct sc_card_driver *sc_get_iso7816_driver(void);
extern struct sc_card_driver *sc_get_miocos_driver(void);
extern struct sc_card_driver *sc_get_mcrd_driver(void);
extern struct sc_card_driver *sc_get_setcos_driver(void);
extern struct sc_card_driver *sc_get_starcos_driver(void);
extern struct sc_card_driver *sc_get_tcos_driver(void);
extern struct sc_card_driver *sc_get_openpgp_driver(void);
extern struct sc_card_driver *sc_get_jcop_driver(void);
extern struct sc_card_driver *sc_get_oberthur_driver(void);

#ifdef  __cplusplus
}
#endif

#endif
