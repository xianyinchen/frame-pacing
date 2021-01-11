/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This is the main interface to the Android Performance Tuner library, also
 * known as Tuning Fork.
 *
 * It is part of the Android Games SDK and produces best results when integrated
 * with the Swappy Frame Pacing Library.
 *
 * See the documentation at
 * https://developer.android.com/games/sdk/performance-tuner/custom-engine for
 * more information on using this library in a native Android game.
 *
 */

/**
 * @defgroup tuningfork Tuning Fork main interface
 * The main interface to use Tuning Fork.
 * @{
 */

#pragma once

#include <jni.h>
#include <stdbool.h>
#include <stdint.h>

#include "common/gamesdk_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @cond INTERNAL */

#define TUNINGFORK_MAJOR_VERSION 1
#define TUNINGFORK_MINOR_VERSION 1
#define TUNINGFORK_BUGFIX_VERSION 4
#define TUNINGFORK_PACKED_VERSION                            \
    ANDROID_GAMESDK_PACKED_VERSION(TUNINGFORK_MAJOR_VERSION, \
                                   TUNINGFORK_MINOR_VERSION, \
                                   TUNINGFORK_BUGFIX_VERSION)

// Internal macros to generate a symbol to track TuningFork version, do not use
// directly.
#define TUNINGFORK_VERSION_CONCAT_NX(PREFIX, MAJOR, MINOR) \
    PREFIX##_##MAJOR##_##MINOR
#define TUNINGFORK_VERSION_CONCAT(PREFIX, MAJOR, MINOR) \
    TUNINGFORK_VERSION_CONCAT_NX(PREFIX, MAJOR, MINOR)
#define TUNINGFORK_VERSION_SYMBOL                                           \
    TUNINGFORK_VERSION_CONCAT(TuningFork_version, TUNINGFORK_MAJOR_VERSION, \
                              TUNINGFORK_MINOR_VERSION)

/** @endcond */

/**
 * @brief Instrument keys indicating time periods within a frame.
 *  Keys 64000-65535 are reserved
 */
enum TuningFork_InstrumentKeys {
    TFTICK_USERDEFINED_BASE = 0,
    TFTICK_RAW_FRAME_TIME =
        64000,  ///< If GPU time is available, thisis MAX(CPU_TIME,GPU_TIME)
                ///< If not, this is the same as PACED_FRAME_TIME
    TFTICK_PACED_FRAME_TIME =
        64001,  ///< Frame time between ends of eglSwapBuffers calls or
                ///< Vulkan queue present.
    TFTICK_CPU_TIME =
        64002,  ///< The time between frame start and the call to Swappy_swap.
    TFTICK_GPU_TIME =
        64003  ///< The time between buffer swap and GPU fence triggering.
};

/**
 * @brief A series of bytes representing a serialized protocol buffer.
 * @see TuningFork_CProtobufSerialization_free for how to deallocate
 * the memory once finished with the buffer.
 */
typedef struct TuningFork_CProtobufSerialization {
    uint8_t* bytes;  /// Array of bytes.
    uint32_t size;   /// Size of array.
    /// Deallocation callback (may be NULL if not owned).
    void (*dealloc)(struct TuningFork_CProtobufSerialization*);
} TuningFork_CProtobufSerialization;

/// The instrumentation key identifies a tick point within a frame or a trace
/// segment
typedef uint16_t TuningFork_InstrumentKey;
/// A trace handle used in TuningFork_startTrace
typedef uint64_t TuningFork_TraceHandle;
/// A  handle used in TuningFork_startRecordingLoadingTime
typedef uint64_t TuningFork_LoadingEventHandle;
/// A time as milliseconds past the epoch.
typedef uint64_t TuningFork_TimePoint;
/// A duration in nanoseconds.
typedef uint64_t TuningFork_Duration;

/**
 * @brief All the error codes that can be returned by Tuning Fork functions.
 */
typedef enum TuningFork_ErrorCode {
    TUNINGFORK_ERROR_OK = 0,  ///< No error
    TUNINGFORK_ERROR_NO_SETTINGS =
        1,  ///< No tuningfork_settings.bin found in assets/tuningfork.
    TUNINGFORK_ERROR_NO_SWAPPY =
        2,  ///< Not able to find the required Swappy functions.
    TUNINGFORK_ERROR_INVALID_DEFAULT_FIDELITY_PARAMS =
        3,  ///< `fpDefaultFileNum` is out of range.
    TUNINGFORK_ERROR_NO_FIDELITY_PARAMS =
        4,  ///< No fidelity parameters found at initialization.
    TUNINGFORK_ERROR_TUNINGFORK_NOT_INITIALIZED =
        5,  ///< A call was made before Tuning Fork was initialized.
    TUNINGFORK_ERROR_INVALID_ANNOTATION =
        6,  ///< Invalid parameter to `TuningFork_setCurrentAnnotation`.
    TUNINGFORK_ERROR_INVALID_INSTRUMENT_KEY =
        7,  ///< Invalid instrument key passed to a tick function.
    TUNINGFORK_ERROR_INVALID_TRACE_HANDLE =
        8,  ///< Invalid handle passed to `TuningFork_startTrace`.
    TUNINGFORK_ERROR_TIMEOUT =
        9,  ///< Timeout in request for fidelity parameters.
    TUNINGFORK_ERROR_BAD_PARAMETER = 10,      ///< Generic bad parameter.
    TUNINGFORK_ERROR_B64_ENCODE_FAILED = 11,  ///< Could not encode a protobuf.
    TUNINGFORK_ERROR_JNI_BAD_VERSION = 12,    ///< Jni error - obsolete
    TUNINGFORK_ERROR_JNI_BAD_THREAD = 13,     ///< Jni error - obsolete
    TUNINGFORK_ERROR_JNI_BAD_ENV = 14,        ///< Jni error - obsolete
    TUNINGFORK_ERROR_JNI_EXCEPTION =
        15,  ///< Jni error - an exception was thrown. See logcat output.
    TUNINGFORK_ERROR_JNI_BAD_JVM = 16,  ///< Jni error - obsolete
    TUNINGFORK_ERROR_NO_CLEARCUT = 17,  ///< Obsolete
    TUNINGFORK_ERROR_NO_FIDELITY_PARAMS_IN_APK =
        18,  ///< No dev_tuningfork_fidelityparams_#.bin found in
             ///< assets/tuningfork.
    TUNINGFORK_ERROR_COULDNT_SAVE_OR_DELETE_FPS =
        19,  ///< Error calling `TuningFork_saveOrDeleteFidelityParamsFile`.
    TUNINGFORK_ERROR_PREVIOUS_UPLOAD_PENDING =
        20,  ///< Can't upload since another request is pending.
    TUNINGFORK_ERROR_UPLOAD_TOO_FREQUENT =
        21,  ///< Too frequent calls to `TuningFork_flush`.
    TUNINGFORK_ERROR_NO_SUCH_KEY =
        22,  ///< No such key when accessing file cache.
    TUNINGFORK_ERROR_BAD_FILE_OPERATION = 23,  ///< General file error.
    TUNINGFORK_ERROR_BAD_SETTINGS =
        24,  ///< Invalid tuningfork_settings.bin file.
    TUNINGFORK_ERROR_ALREADY_INITIALIZED =
        25,  ///< TuningFork_init was called more than once.
    TUNINGFORK_ERROR_NO_SETTINGS_ANNOTATION_ENUM_SIZES =
        26,  ///< Missing part of tuningfork_settings.bin.
    TUNINGFORK_ERROR_DOWNLOAD_THREAD_ALREADY_STARTED =
        27,  ///< `TuningFork_startFidelityParamDownloadThread`
             ///< was called more than once, or called when TuningFork_init has
             ///< already started download.
    TUNINGFORK_ERROR_PLATFORM_NOT_SUPPORTED = 28,  ///< Obsolete.
    TUNINGFORK_ERROR_GENERATE_TUNING_PARAMETERS_ERROR =
        29,  ///< An error occurred parsing the response to
             ///< generateTuningParameters
    TUNINGFORK_ERROR_GENERATE_TUNING_PARAMETERS_RESPONSE_NOT_SUCCESS =
        30,  ///< The response from generateTuningParameters was not a success
             ///< code
    TUNINGFORK_ERROR_NO_MORE_SPACE_FOR_LOADING_TIME_DATA =
        31,  ///< Not enough space for metric data was allocated at start-up:
             ///< increase Settings.max_num_metrics.loading_time
    TUNINGFORK_ERROR_NO_MORE_SPACE_FOR_FRAME_TIME_DATA =
        32,  ///< Not enough space for metric data was allocated at start-up:
    ///< increase Settings.max_num_metrics.frame_time or check
    ///< max_num_instrument_keys
    TUNINGFORK_ERROR_INVALID_LOADING_HANDLE =
        33,  ///< Invalid handle passed to
             ///< `TuningFork_startRecordingLoadingTime`.
    TUNINGFORK_ERROR_DUPLICATE_START_LOADING_EVENT =
        34,  ///< TuningFork_startRecordingLoadingTime was called with the same
             ///< parameters twice without a stop function inbetween.
    TUNINGFORK_ERROR_METERED_CONNECTION_DISALLOWED =
        35,  ///< An HTTP request could not be made because there is no
             ///< unmetered connection available.
    // Error codes 100-150 are reserved for engines integrations.
} TuningFork_ErrorCode;

/**
 * @defgroup TuningFork_Cache Tuning Fork cache utilities
 * Optional persistent cache object to use with Tuning Fork.
 * @{
 */

/**
 * @brief Pointer to a function that can be attached to TuningFork_Cache::get
 *
 * Function that will be called to get a value for a key.
 * @see TuningFork_Cache
 */
typedef TuningFork_ErrorCode (*TuningFork_CacheGet)(
    uint64_t key, TuningFork_CProtobufSerialization* value, void* user_data);

/**
 * @brief Pointer to a function that can be attached to TuningFork_Cache::set
 *
 * Function that will be called to set a value for a key.
 * @see TuningFork_Cache
 */
typedef TuningFork_ErrorCode (*TuningFork_CacheSet)(
    uint64_t key, const TuningFork_CProtobufSerialization* value,
    void* user_data);

/**
 * @brief Pointer to a function that can be attached to TuningFork_Cache::remove
 *
 * Function that will be called to remove an entry in the cache.
 * @see TuningFork_Cache
 */
typedef TuningFork_ErrorCode (*TuningFork_CacheRemove)(uint64_t key,
                                                       void* user_data);

/**
 * @brief An object used to cache upload data when no connection is available.
 *  If you do not supply one of these, data is saved to a temporary file.
 */
typedef struct TuningFork_Cache {
    void* user_data;          ///< Data passed to each callback.
    TuningFork_CacheSet set;  ///< Function to set a value for a key.
    TuningFork_CacheGet get;  ///< Function to get a value for a key.
    TuningFork_CacheRemove
        remove;  ///< Function to remove an entry in the cache.
} TuningFork_Cache;

/** @} */

/**
 * @brief Pointer to a function that can be attached to
 * TuningFork_Settings::fidelity_params_callback
 *
 * Function that will be called with the fidelity parameters that are
 * downloaded.
 * @see TuningFork_Settings
 *
 * Fidelity parameters are serializations of FidelityParams messages defined in
 * a game's dev_tuningfork.proto file. The structure of this message is
 * completely up to the developer, with limitations outlined in the integration
 * guide.
 *
 */
typedef void (*TuningFork_FidelityParamsCallback)(
    const TuningFork_CProtobufSerialization*);

/**
 * @brief Pointer to a function that can be passed to
 * TuningFork_setUploadCallback.

 * @param message UTF-8 string containing the JSON uploaded.
 * @param size Number of bytes in message.
 *
 * Function that will be called on a separate thread every
 * time TuningFork performs an upload.
 * @see TuningFork_Settings
 */
typedef void (*TuningFork_UploadCallback)(const char* message, size_t size);

struct SwappyTracer;

/**
 * @brief Pointer to Swappy_injectTracers that can be attached
 * to TuningFork_Settings::swappy_tracer_fn.
 * @see TuningFork_Settings
 */
typedef void (*SwappyTracerFn)(const struct SwappyTracer*);

/**
 * @brief Limits on the number of metrics of each type.
 * Zero any values to get the default for that type:

 * Frame time: min(64, the maximum number of annotation combinations) *
 num_instrument_keys.

 * Loading time: 32.

 * Memory: 15 possible memory metrics.
 */
typedef struct TuningFork_MetricLimits {
    uint32_t frame_time;
    uint32_t loading_time;
    uint32_t memory;
} TuningFork_MetricLimits;

/**
 * @brief Initialization settings
 *   Zero any values that are not being used.
 */
typedef struct TuningFork_Settings {
    /**
     * Cache object to be used for upload data persistence.
     * If null, data is persisted to /data/local/tmp/tuningfork
     */
    const TuningFork_Cache* persistent_cache;
    /**
     * The address of the Swappy_injectTracers function.
     * If this is null, you need to call TuningFork_tick yourself.
     * If it is set, telemetry for 4 instrument keys is automatically recorded.
     */
    SwappyTracerFn swappy_tracer_fn;
    /**
     * Callback
     * If set, this is called with the fidelity parameters that are downloaded.
     * If null, you need to call TuningFork_getFidelityParameters yourself.
     */
    TuningFork_FidelityParamsCallback fidelity_params_callback;
    /**
     * A serialized protobuf containing the fidelity parameters to be uploaded
     *  for training.
     * Set this to nullptr if you are not using training mode.
     * In training mode, these parameters are taken to be the parameters used
     * within the game and they are used to help suggest parameter changes for
     * different devices. Note that these override the default parameters loaded
     * from the APK at startup.
     */
    const TuningFork_CProtobufSerialization* training_fidelity_params;
    /**
     * A null-terminated UTF-8 string containing the endpoint that Tuning Fork
     * will connect to for parameter, upload and debug requests. This overrides
     * the value in base_uri in the settings proto and is intended for debugging
     * purposes only.
     */
    const char* endpoint_uri_override;
    /**
     * The version of Swappy that swappy_tracer_fn comes from
     */
    uint32_t swappy_version;
    /**
     * The number of each metric that is allowed to be allocated at any given
     * time. If any element is zero, the default for that metric type will be
     * used. Memory for all metrics is allocated up-front at initialization.
     * When all metrics of a given type are allocated, further requested metrics
     * will not be added and data will be lost.
     */
    TuningFork_MetricLimits max_num_metrics;
} TuningFork_Settings;

/**
 * @brief Deallocate any memory owned by the procol buffer serialization.
 * @param ser A protocol buffer serialization
 */
inline void TuningFork_CProtobufSerialization_free(
    TuningFork_CProtobufSerialization* ser) {
    if (ser->dealloc) {
        ser->dealloc(ser);
        ser->dealloc = NULL;
    }
}

/** @cond INTERNAL */

// Internal init function. Do not call directly.
TuningFork_ErrorCode TuningFork_init_internal(
    const TuningFork_Settings* settings, JNIEnv* env, jobject context);

// Internal function to track TuningFork version bundled in a binary. Do not
// call directly. If you are getting linker errors related to
// TuningFork_version_x_y, you probably have a mismatch between the header used
// at compilation and the actually library used by the linker.
void TUNINGFORK_VERSION_SYMBOL();

/** @endcond */

/**
 * @brief Initialize Tuning Fork. This must be called before any other
 * functions.
 *
 * The library will load histogram and annotation settings from your
 * tuningfork_settings.bin file.
 * @see TuningFork_Settings for the semantics of how other settings change
 * initialization behaviour.
 *
 * @param settings a TuningFork_Settings structure
 * @param env a JNIEnv
 * @param context the app context
 *
 * @return TUNINGFORK_ERROR_OK if successful,
 * @return TUNINGFORK_ERROR_NO_SETTINGS if no settings could be found,
 * @return TUNINGFORK_ERROR_BAD_SETTINGS if your tuningfork_settings.bin file
 * was invalid or
 * @return TUNINGFORK_ERROR_ALREADY_INITIALIZED if tuningfork was already
 * initialized.
 */
static inline TuningFork_ErrorCode TuningFork_init(
    const TuningFork_Settings* settings, JNIEnv* env, jobject context) {
    // This call ensures that the header and the linked library are from the
    // same version (if not, a linker error will be triggered because of an
    // undefined symbol).
    TUNINGFORK_VERSION_SYMBOL();
    return TuningFork_init_internal(settings, env, context);
}

// The functions below will return TUNINGFORK_ERROR_TUNINGFORK_NOT_INITIALIZED
// if TuningFork_init
//  has not first been called.

/**
 * @brief A blocking call to get fidelity parameters from the server.
 * You do not need to call this if you pass in a fidelity_params_callback as
 * part of the settings to TuningFork_init. Note that once fidelity parameters
 * are downloaded, any timing information is recorded as being associated with
 * those parameters. If you subsequently call GetFidelityParameters and a new
 * set of parameters is downloaded, any data that is already collected will be
 * submitted to the backend. Ownership of 'params' is transferred to the caller,
 * so they must call params->dealloc when they are done with it. The parameter
 * request is sent to:
 *  ${url_base}+'applications/'+package_name+'/apks/'+version_number+':generateTuningParameters'.
 * @param defaultParams these will be assumed current if no parameters could be
 * downloaded.
 * @param[out] params
 * @param timeout_ms time to wait before returning from this call when no
 * connection can be made. If zero or negative, the value in
 * Settings.initial_request_timeout_ms is used.
 * @return TUNINGFORK_ERROR_TIMEOUT if there was a timeout before params could
 * be downloaded.
 * @return TUNINGFORK_ERROR_OK on success.
 */
TuningFork_ErrorCode TuningFork_getFidelityParameters(
    const TuningFork_CProtobufSerialization* defaultParams,
    TuningFork_CProtobufSerialization* params, uint32_t timeout_ms);

/**
 * @brief Set the current annotation.
 * @param annotation the protobuf serialization of the current annotation.
 * @return TUNINGFORK_ERROR_INVALID_ANNOTATION if annotation is inconsistent
 * with the settings.
 * @return TUNINGFORK_ERROR_OK on success.
 */
TuningFork_ErrorCode TuningFork_setCurrentAnnotation(
    const TuningFork_CProtobufSerialization* annotation);

/**
 * @brief Record a frame tick that will be associated with the instrumentation
 * key and the current annotation. NB: calling the tick or trace functions from
 * different threads is allowed, but a single instrument key should always be
 * ticked from the same thread.
 * @param key an instrument key
 * @see the reserved instrument keys above
 * @return TUNINGFORK_ERROR_INVALID_INSTRUMENT_KEY if the instrument key is
 * invalid.
 * @return TUNINGFORK_ERROR_OK on success.
 */
TuningFork_ErrorCode TuningFork_frameTick(TuningFork_InstrumentKey key);

/**
 * @brief Record a frame tick using an external time, rather than system time.
 * @param key an instrument key
 * @see the reserved instrument keys above
 * @param dt the duration you wish to record (in nanoseconds)
 * @return TUNINGFORK_ERROR_INVALID_INSTRUMENT_KEY if the instrument key is
 * invalid.
 * @return TUNINGFORK_ERROR_OK on success.
 */
TuningFork_ErrorCode TuningFork_frameDeltaTimeNanos(
    TuningFork_InstrumentKey key, TuningFork_Duration dt);

/**
 * @brief Start a trace segment.
 * @param key an instrument key
 * @see the reserved instrument keys above
 * @param[out] handle this is filled with a new handle on success.
 * @return TUNINGFORK_ERROR_INVALID_INSTRUMENT_KEY if the instrument key is
 * invalid.
 * @return TUNINGFORK_ERROR_OK on success.
 */
TuningFork_ErrorCode TuningFork_startTrace(TuningFork_InstrumentKey key,
                                           TuningFork_TraceHandle* handle);

/**
 * @brief Stop and record a trace segment.
 * @param handle this is a handle previously returned by TuningFork_startTrace
 * @return TUNINGFORK_ERROR_INVALID_TRACE_HANDLE if the handle is invalid.
 * @return TUNINGFORK_ERROR_OK on success.
 */
TuningFork_ErrorCode TuningFork_endTrace(TuningFork_TraceHandle handle);

/**
 * @brief Force upload of the current histograms.
 * @return TUNINGFORK_ERROR_OK if the upload could be initiated.
 * @return TUNINGFORK_ERROR_PREVIOUS_UPLOAD_PENDING if there is a previous
 * upload blocking this one.
 * @return TUNINGFORK_ERROR_UPLOAD_TOO_FREQUENT if less than a minute has
 * elapsed since the previous upload.
 */
TuningFork_ErrorCode TuningFork_flush();

/**
 * @brief Set a callback to be called on a separate thread every time TuningFork
 * performs an upload.
 * @param cbk
 * @return TUNINGFORK_ERROR_OK on success.
 * @return TUNINGFORK_ERROR_TUNINGFORK_NOT_INITIALIZED if Tuning Fork wasn't
 * initialized.
 */
TuningFork_ErrorCode TuningFork_setUploadCallback(
    TuningFork_UploadCallback cbk);

/**
 * @brief Clean up all memory owned by Tuning Fork and kill any threads.
 * @return TUNINGFORK_ERROR_OK on success.
 * @return TUNINGFORK_ERROR_TUNINGFORK_NOT_INITIALIZED if Tuning Fork wasn't
 * initialized.
 */
TuningFork_ErrorCode TuningFork_destroy();

/**
 * @brief Set the currently active fidelity parameters.
 * This function overrides any parameters that have been downloaded if in
 * experiment mode. Use when, for instance, the player has manually changed game
 * quality  settings. This flushes (i.e. uploads) any data associated with any
 * previous parameters.
 * @param params The protocol buffer encoded parameters.
 * @return TUNINGFORK_ERROR_OK on success.
 * @return TUNINGFORK_ERROR_TUNINGFORK_NOT_INITIALIZED if Tuning Fork wasn't
 * initialized.
 */
TuningFork_ErrorCode TuningFork_setFidelityParameters(
    const TuningFork_CProtobufSerialization* params);

/**
 * @brief Enable or disable memory telemetry recording.
 * By default, memory telemetry recording is turned *off* at initialization and
 * currently the memory statistics are not shown in the Play UI, so it is not
 * recommended to enable.
 * @param enable If true, memory recording is enabled, if false it is disabled.
 * @return TUNINGFORK_ERROR_OK on success.
 * @return TUNINGFORK_ERROR_TUNINGFORK_NOT_INITIALIZED if Tuning Fork wasn't
 * initialized.
 */
TuningFork_ErrorCode TuningFork_enableMemoryRecording(bool enable);

/**
 * @brief Metadata recorded with a loading time event
 */
typedef struct TuningFork_LoadingTimeMetadata {
    enum LoadingState {
        UNKNOWN_STATE = 0,
        // The first time the game is run
        FIRST_RUN = 1,
        // App is not backgrounded
        COLD_START = 2,
        // App is backgrounded
        WARM_START = 3,
        // App is backgrounded, least work needed
        HOT_START = 4,
        // Asset loading between levels
        INTER_LEVEL = 5
    } state;
    enum LoadingSource {
        UNKNOWN_SOURCE = 0,
        // Uncompressing data.
        MEMORY = 1,
        // Reading assets from APK bundle.
        APK = 2,
        // Reading assets from device storage.
        DEVICE_STORAGE = 3,
        // Reading assets from external storage, e.g. SD card.
        EXTERNAL_STORAGE = 4,
        // Loading assets from the network.
        NETWORK = 5,
        // Shader compilation.
        SHADER_COMPILATION = 6,
        // Time spent between process starting and onCreate.
        PRE_ACTIVITY = 7,
        // Total time spent between process starting and first render frame.
        FIRST_TOUCH_TO_FIRST_FRAME = 8
    } source;
    int32_t compression_level;  // 0 = no compression, 100 = max compression
    enum NetworkConnectivity {
        UNKNOWN = 0,
        WIFI = 1,
        CELLULAR_NETWORK = 2
    } network_connectivity;
    uint64_t network_transfer_speed_bps;  // bandwidth in bits per second
    uint64_t network_latency_ns;          // latency in nanoseconds
} TuningFork_LoadingTimeMetadata;

/**
 * @brief Record a loading time event.
 * @param time_ns The time taken for loading event in nanoseconds
 * @param eventMetadata A LoadingTimeMetadata structure
 * @param eventMetadataSize Size in bytes of the LoadingTimeMetadata structure
 * @param annotation The annotation to use with this event.
 **/
TuningFork_ErrorCode TuningFork_recordLoadingTime(
    uint64_t time_ns, const TuningFork_LoadingTimeMetadata* eventMetadata,
    uint32_t eventMetadataSize,
    const TuningFork_CProtobufSerialization* annotation);

/**
 * @brief Record the start of a loading event.
 * @param eventMetadata A LoadingTimeMetadata structure.
 * @param eventMetadataSize Size in bytes of the LoadingTimeMetadata structure
 *(for versioning of the structure).
 * @param annotation The annotation to use with this event.
 * @param[out] handle A handle for this event.
 **/
TuningFork_ErrorCode TuningFork_startRecordingLoadingTime(
    const TuningFork_LoadingTimeMetadata* eventMetadata,
    uint32_t eventMetadataSize,
    const TuningFork_CProtobufSerialization* annotation,
    TuningFork_LoadingEventHandle* handle);

/**
 * @brief Record the end of a loading event.
 * @param handle A handle generated by startRecordingLoadingTime
 **/
TuningFork_ErrorCode TuningFork_stopRecordingLoadingTime(
    TuningFork_LoadingEventHandle handle);

/**
 * @brief The set of states that the TuningFork_reportLifecycleEvent method
 * accepts.
 */
typedef enum TuningFork_LifecycleState {
    TUNINGFORK_STATE_UNINITIALIZED = 0,
    TUNINGFORK_STATE_ONCREATE = 1,
    TUNINGFORK_STATE_ONSTART = 2,
    TUNINGFORK_STATE_ONSTOP = 3,
    TUNINGFORK_STATE_ONDESTROY = 4,

} TuningFork_LifecycleState;

/**
 * @brief Record the state change of the app's lifecycle.
 * This method should be called every time the onCreate(), onStart(), onStop()
 * and onDestroy() methods are triggered for the app. This will help tuning
 * fork keep track of exactly when the app is on foreground or background.
 * @param state The new lifecycle state of the app
 * @return TUNINGFORK_ERROR_OK on success.
 * @return TUNINGFORK_ERROR_TUNINGFORK_NOT_INITIALIZED if Tuning Fork wasn't
 * initialized.
 */
TuningFork_ErrorCode TuningFork_reportLifecycleEvent(
    TuningFork_LifecycleState state);

#ifdef __cplusplus
}
#endif

/** @} */
