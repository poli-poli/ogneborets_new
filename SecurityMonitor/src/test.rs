#[cfg(test)]
mod tests {
    use super::super::*;

    #[test]
    fn test_check_operation_valid() {
        let mut valid_operation = serde_json::json!({
        
            "source": "FlightTaskAuthenticator",
            "deliver_to": "CentralControlSystem",
            "operation": "start_extinguishing"
        });
       
        assert_eq!(check_operation(&valid_operation), true);
        
        valid_operation["source"] = "CentralControlSystem".into();
        valid_operation["deliver_to"] = "ExtinguishingControlSystem".into();

        assert_eq!(check_operation(&valid_operation), true);
    }

    #[test]
    fn test_check_operation_invalid_source() {
        let invalid_operation = serde_json::json!({
            "source": "UnknownSystem",
            "deliver_to": "CentralControlSystem",
            "operation": "extinguishing_completed"
        });

        assert_eq!(check_operation(&invalid_operation), false);
    }

    #[test]
    fn test_check_operation_invalid_operation() {
        let invalid_operation = serde_json::json!({
            "source": "BatteryChargeControlSystem",
            "deliver_to": "CentralControlSystem",
            "operation": "invalid_operation"
        });

        assert_eq!(check_operation(&invalid_operation), false);
    }

    #[test]
    fn test_check_operation_missing_field() {
        let missing_field_operation = serde_json::json!({
            "source": "FlightTaskAuthenticator",
            "operation": "start_extinguishing"
            // deliver_to field is missing
        });

        assert_eq!(check_operation(&missing_field_operation), false);
    }
}
