#pragma once

#include <array>
#include <functional>
#include <limits>
#include <memory>
#include <string>

#include "plugin_base.h"

namespace mavsdk {

class System;
class ActionImpl;

/**
 * @brief Enable simple actions such as arming, taking off, and landing.
 */
class Action : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto action = std::make_shared<Action>(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Action(System& system);

    /**
     * @brief Destructor (internal use only).
     */
    ~Action();

    /**
     * @brief Possible results returned for action requests.
     */
    enum class Result {
        Unknown, /**< @brief Unknown error. */
        Success, /**< @brief Success: the action command was accepted by the vehicle. */
        NoSystem, /**< @brief No system is connected. */
        ConnectionError, /**< @brief Connection error. */
        Busy, /**< @brief Vehicle is busy. */
        CommandDenied, /**< @brief Command refused by vehicle. */
        CommandDeniedLandedStateUnknown, /**< @brief Command refused because landed state is
                                            unknown. */
        CommandDeniedNotLanded, /**< @brief Command refused because vehicle not landed. */
        Timeout, /**< @brief Request timed out. */
        VtolTransitionSupportUnknown, /**< @brief Hybrid/VTOL transition refused because VTOL
                                         support is unknown. */
        NoVtolTransitionSupport, /**< @brief Vehicle does not support hybrid/VTOL transitions. */
        ParameterError, /**< @brief Error getting or setting parameter. */
    };

    /**
     * @brief Callback type for asynchronous Action calls.
     */
    typedef std::function<void(Result)> result_callback_t;

    /**
     * @brief Send command to arm the drone.
     *
     * Arming a drone normally causes motors to spin at idle.
     * Before arming take all safety precautions and stand clear of the drone!
     */
    void arm_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for arm_async().
     *
     * @return Result of request.
     */
    Result arm() const;

    /**
     * @brief Send command to disarm the drone.
     *
     * This will disarm a drone that considers itself landed. If flying, the drone should
     * reject the disarm command. Disarming means that all motors will stop.
     */
    void disarm_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for disarm_async().
     *
     * @return Result of request.
     */
    Result disarm() const;

    /**
     * @brief Send command to take off and hover.
     *
     * This switches the drone into position control mode and commands
     * it to take off and hover at the takeoff altitude.
     *
     * Note that the vehicle must be armed before it can take off.
     */
    void takeoff_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for takeoff_async().
     *
     * @return Result of request.
     */
    Result takeoff() const;

    /**
     * @brief Send command to land at the current position.
     *
     * This switches the drone to 'Land' flight mode.
     */
    void land_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for land_async().
     *
     * @return Result of request.
     */
    Result land() const;

    /**
     * @brief Send command to reboot the drone components.
     *
     * This will reboot the autopilot, companion computer, camera and gimbal.
     */
    void reboot_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for reboot_async().
     *
     * @return Result of request.
     */
    Result reboot() const;

    /**
     * @brief *
     * Send command to shut down the drone components.
     *
     * This will shut down the autopilot, onboard computer, camera and gimbal.
     * This command should only be used when the autopilot is disarmed and autopilots commonly
     * reject it if they are not already ready to shut down.
     */
    void shutdown_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for shutdown_async().
     *
     * @return Result of request.
     */
    Result shutdown() const;

    /**
     * @brief Send command to kill the drone.
     *
     * This will disarm a drone irrespective of whether it is landed or flying.
     * Note that the drone will fall out of the sky if this command is used while flying.
     */
    void kill_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for kill_async().
     *
     * @return Result of request.
     */
    Result kill() const;

    /**
     * @brief Send command to return to the launch (takeoff) position and land.
     *
     * This switches the drone into [RTL mode](https://docs.px4.io/en/flight_modes/rtl.html) which
     * generally means it will rise up to a certain altitude to clear any obstacles before heading
     * back to the launch (takeoff) position and land there.
     */
    void return_to_launch_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for return_to_launch_async().
     *
     * @return Result of request.
     */
    Result return_to_launch() const;

    /**
     * @brief *
     * Send command to move the vehicle to a specific global position.
     *
     * The latitude and longitude are given in degrees (WGS84 frame) and the altitude
     * in meters AMSL (above mean sea level).
     *
     * The yaw angle is in degrees (frame is NED, 0 is North, positive is clockwise).
     */
    void goto_location_async(
        double latitude_deg,
        double longitude_deg,
        float absolute_altitude_m,
        float yaw_deg,
        const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for goto_location_async().
     *
     * @return Result of request.
     */
    Result goto_location(
        double latitude_deg, double longitude_deg, float absolute_altitude_m, float yaw_deg) const;

    /**
     * @brief Send command to transition the drone to fixedwing.
     *
     * The associated action will only be executed for VTOL vehicles (on other vehicle types the
     * command will fail). The command will succeed if called when the vehicle
     * is already in fixedwing mode.
     */
    void transition_to_fixedwing_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for transition_to_fixedwing_async().
     *
     * @return Result of request.
     */
    Result transition_to_fixedwing() const;

    /**
     * @brief Send command to transition the drone to multicopter.
     *
     * The associated action will only be executed for VTOL vehicles (on other vehicle types the
     * command will fail). The command will succeed if called when the vehicle
     * is already in multicopter mode.
     */
    void transition_to_multicopter_async(const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for transition_to_multicopter_async().
     *
     * @return Result of request.
     */
    Result transition_to_multicopter() const;

    /**
     * @brief Callback type for get_takeoff_altitude_async.
     */
    typedef std::function<void(Result, float)> altitude_callback_t;

    /**
     * @brief Get the takeoff altitude (in meters above ground).
     */
    void get_takeoff_altitude_async(const altitude_callback_t callback);

    /**
     * @brief Synchronous wrapper for get_takeoff_altitude_async().
     *
     * @return Result of request.
     */
    std::pair<Result, float> get_takeoff_altitude() const;

    /**
     * @brief Set takeoff altitude (in meters above ground).
     */
    void set_takeoff_altitude_async(float altitude, const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for set_takeoff_altitude_async().
     *
     * @return Result of request.
     */
    Result set_takeoff_altitude(float altitude) const;

    /**
     * @brief Callback type for get_maximum_speed_async.
     */
    typedef std::function<void(Result, float)> speed_callback_t;

    /**
     * @brief Get the vehicle maximum speed (in metres/second).
     */
    void get_maximum_speed_async(const speed_callback_t callback);

    /**
     * @brief Synchronous wrapper for get_maximum_speed_async().
     *
     * @return Result of request.
     */
    std::pair<Result, float> get_maximum_speed() const;

    /**
     * @brief Set vehicle maximum speed (in metres/second).
     */
    void set_maximum_speed_async(float speed, const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for set_maximum_speed_async().
     *
     * @return Result of request.
     */
    Result set_maximum_speed(float speed) const;

    /**
     * @brief Callback type for get_return_to_launch_altitude_async.
     */
    typedef std::function<void(Result, float)> relative_altitude_m_callback_t;

    /**
     * @brief Get the return to launch minimum return altitude (in meters).
     */
    void get_return_to_launch_altitude_async(const relative_altitude_m_callback_t callback);

    /**
     * @brief Synchronous wrapper for get_return_to_launch_altitude_async().
     *
     * @return Result of request.
     */
    std::pair<Result, float> get_return_to_launch_altitude() const;

    /**
     * @brief Set the return to launch minimum return altitude (in meters).
     */
    void set_return_to_launch_altitude_async(
        float relative_altitude_m, const result_callback_t callback);

    /**
     * @brief Synchronous wrapper for set_return_to_launch_altitude_async().
     *
     * @return Result of request.
     */
    Result set_return_to_launch_altitude(float relative_altitude_m) const;

    /**
     * @brief Returns a human-readable English string for a Result.
     *
     * @param result The enum value for which a human readable string is required.
     * @return Human readable string for the Result.
     */
    static const char* result_str(Result result);

    /**
     * @brief Copy constructor (object is not copyable).
     */
    Action(const Action&) = delete;

    /**
     * @brief Equality operator (object is not copyable).
     */
    const Action& operator=(const Action&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<ActionImpl> _impl;
};

} // namespace mavsdk