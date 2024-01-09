use serde_json::Value;


pub fn check_operation(details: &Value) -> bool {
    let src = details.get("source").and_then(Value::as_str);
    let dst = details.get("deliver_to").and_then(Value::as_str);
    let opr = details.get("operation").and_then(Value::as_str);

    if src.is_none() || dst.is_none() || opr.is_none() {
        return false;
    }

    let src = src.unwrap();
    let dst = dst.unwrap();
    let opr = opr.unwrap();

    println!("[info] checking policies for operation: {}->{}: {}", src, dst, opr);

    match (src, dst, opr) {
        ("ComplexingSystem", "CentralControlSystem", "request_coordinates") => true,
        ("CentralControlSystem", "MovementControlSystem", "move_to_area") => true,
        ("MovementControlSystem", "CentralControlSystem", "movement_completed") => true,
        ("CentralControlSystem", "ExtinguishingControlSystem", "start_extinguishing") => true,
        ("CentralControlSystem", "ExtinguishingControlSystem", "start_ignition") => true,
        ("CentralControlSystem", "SituationControlSystem", "check_extinguishing_status") => true,
        ("CentralControlSystem", "ExtinguishingControlSystem", "stop_extinguishing") => true,
        ("CentralControlSystem", "BatteryChargeControlSystem", "check_battery") => true,
        ("GNSSNavigationSystem", "ComplexingSystem", "send_coordinates") => true,
        ("INSNavigationSystem", "ComplexingSystem", "send_coordinates") => true,
        ("ComplexingSystem", "CentralControlSystem", "coordinates") => true,
        ("SituationControlSystem", "CentralControlSystem", "extinguishing_status") => true,
        ("ExtinguishingControlSystem", "CentralControlSystem", "extinguishing_completed") => true,
        ("ExtinguishingControlSystem", "CentralControlSystem", "ignition_completed") => true,
        ("CentralControlSystem", "FireIgnitionSystem", "activate_ignition") => true,
        ("FireIgnitionSystem", "CentralControlSystem", "ignition_activated") => true,
        ("CentralControlSystem", "GNSSNavigationSystem", "request_coordinates") => true,
        ("GNSSNavigationSystem", "ComplexingSystem", "coordinates") => true,
        ("INSNavigationSystem", "ComplexingSystem", "coordinates") => true,
        ("CentralControlSystem", "SituationControlSystem", "request_situation_update") => true,
        ("SituationControlSystem", "CentralControlSystem", "situation_update") => true,
        ("CentralControlSystem", "MovementControlSystem", "request_movement_update") => true,
        ("MovementControlSystem", "CentralControlSystem", "movement_update") => true,
        ("CentralControlSystem", "FireExtinguishingSystem", "activate_extinguishing") => true,
        ("FireExtinguishingSystem", "CentralControlSystem", "extinguishing_activated") => true,
        ("BatteryChargeControlSystem", "CentralControlSystem", "battery_status") => true,
        ("FlightTaskAuthenticator", "CentralControlSystem", "start_extinguishing1") => true,
        ("FlightTaskAuthenticator", "ExtinguishingControlSystem", "start_extinguishing2") => true,
        ("Connection", "FlightTaskAuthenticator", "start_extinguishing1") => true,
        ("Connection", "FlightTaskAuthenticator", "start_extinguishing2") => true,
       
        _ => false,
    }
}


